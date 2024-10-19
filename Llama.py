import transformers
import torch
from transformers import AutoTokenizer
import serial
import time
import logging
import re

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# LLaMA model setup
model = "meta-llama/Llama-3.2-3B"
tokenizer = AutoTokenizer.from_pretrained(model)

pipeline = transformers.pipeline(
    "text-generation",
    model=model,
    torch_dtype=torch.float16,
    device_map="auto",
)

# Serial connection setup
ser = serial.Serial('COM5', 9600, timeout=1)  # Replace with your serial port
time.sleep(2)  # Allow time for the serial connection to establish

processed_prompts = set()  # To keep track of processed prompts

# Define keywords or patterns to filter out unwanted messages
error_keywords = ["Error", "TX dropped", "failed", "not valid", "characteristic", "warning"]

while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        if data:
            print(f"Received prompt: {data}")

            # Check if the message contains error keywords
            if any(keyword in data for keyword in error_keywords):
                print("Ignoring error message.")
                continue  # Skip this iteration if it's an error message

            # Remove the "16 :" part from the prompt
            cleaned_prompt = re.sub(r'^\d+\s*:\s*', '', data)  # Regex to remove leading number and colon
            print(f"Cleaned prompt: {cleaned_prompt}\n")

            # Check if the prompt has already been processed
            if cleaned_prompt in processed_prompts:
                print("Prompt has already been processed. Skipping.")
                continue  # Skip to the next iteration

            # Mark this prompt as processed
            processed_prompts.add(cleaned_prompt)

            # Pass the cleaned prompt to the LLaMA model
            sequences = pipeline(
                cleaned_prompt,
                do_sample=True,
                top_k=10,
                num_return_sequences=1,
                eos_token_id=tokenizer.eos_token_id,
                truncation=True,
                max_length=400,
            )

            # Retrieve the generated text
            for seq in sequences:
                result = seq['generated_text'].strip()  # Clean up the output
                print(f"Result: {result}")

                # Optionally, limit the result length for sending
                max_send_length = 50  # Adjust as necessary
                result_to_send = result[:max_send_length]  # Limit to first N characters

                # Send the result back to TinyZero via serial
                ser.write(result_to_send.encode('utf-8') + b'\n')  # Add a newline for clarity

        # Optional: add a small delay to avoid overwhelming the serial connection
        time.sleep(0.1)