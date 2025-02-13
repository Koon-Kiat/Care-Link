/**
 * Returns the color associated with a given status and type.
 *
 * @param {string} status - The status to evaluate.
 * @param {string} type - The type of status (e.g., "fall", "activity", "medication", "panic").
 * @returns {string} The color representing the status.
 *
 */
function getStatusColor(status, type) {
  if (!status) {
    return "gray";
  }
  switch (type) {
    case "fall":
      // Check for specific fall detections
      if (
        status === "SEVERE FALL DETECTED" ||
        status === "MODERATE FALL DETECTED" ||
        status === "MINOR FALL DETECTED"
      ) {
        return "danger";
      }
      return "safe";
    case "activity":
      const activityColors = {
        walking: "warning",
        running: "danger",
        resting: "safe",
        "SEVERE FALL DETECTED!": "danger",
        "MODERATE FALL DETECTED!": "danger",
        "MINOR FALL DETECTED!": "danger",
        default: "safe",
      };
      return activityColors[status] || activityColors.default;
    case "medication":
      return status === "MEDICATION CONFIRMED"
        ? "safe"
        : status === "MEDICATION CANCELLED"
        ? "danger"
        : status === "NOT CONFIRMED"
        ? "warning"
        : "warning";
    case "panic":
      if (status === "HELP: REQUESTED") {
        return "danger";
      } else if (
        status === "HELP: CANCELLED" ||
        status === "HELP: NOT REQUESTED" || // Ensure both with and without exclamation
        status === "false"
      ) {
        return "safe";
      } else {
        return "warning";
      }
    default:
      return status === "safe"
        ? "safe"
        : status === "warning"
        ? "warning"
        : "danger";
  }
}

/**
 * Determines the color code based on the temperature value.
 *
 * @param {number} temp - The temperature value to evaluate.
 * @returns {string} - Returns "danger" if the temperature is above 35,
 *                     "warning" if the temperature is between 30 and 35 (inclusive),
 *                     and "safe" if the temperature is below 30.
 */
function getTemperatureColor(temp) {
  return temp > 35 ? "danger" : temp >= 30 ? "warning" : "safe";
}

/**
 * Fetches the sensor data from the server and updates the UI.
 */
async function fetchData() {
  try {
    const response = await fetch("/api/sensordata");
    const data = await response.json();

    // Update Fall Status
    const fallStatusEl = document.getElementById("fallStatus");
    const fallIndicatorEl = document.getElementById("fallIndicator");

    const fallStatuses = [
      "SEVERE FALL DETECTED",
      "MODERATE FALL DETECTED",
      "MINOR FALL DETECTED",
    ];

    if (fallStatuses.includes(data.fall_status.toLowerCase())) {
      fallStatusEl.textContent = "FALL DETECTED";
      fallStatusEl.className = "value danger";
      fallIndicatorEl.className = "status-indicator bg-danger";
    } else if (
      ["walking", "running", "resting"].includes(
        data.activity_status.toLowerCase()
      )
    ) {
      fallStatusEl.textContent = "SAFE";
      fallStatusEl.className = "value safe";
      fallIndicatorEl.className = "status-indicator bg-safe";
    } else {
      fallStatusEl.textContent = "FALL DETECTED";
      fallStatusEl.className = "value danger";
      fallIndicatorEl.className = "status-indicator bg-danger";
    }

    // Update Medication Status
    const medStatusEl = document.getElementById("medStatus");
    const medStatusClass = getStatusColor(data.med_status, "medication");
    medStatusEl.textContent = data.med_status;
    medStatusEl.className = `value ${medStatusClass}`;
    document.getElementById(
      "medIndicator"
    ).className = `status-indicator bg-${medStatusClass}`;

    // Update Panic Status
    const panicStatusEl = document.getElementById("panicStatus");
    const panicStatusClass = getStatusColor(data.panic_status, "panic");
    panicStatusEl.textContent = data.panic_status;
    panicStatusEl.className = `value ${panicStatusClass}`;
    document.getElementById(
      "panicIndicator"
    ).className = `status-indicator bg-${panicStatusClass}`;

    // Update Activity Status
    const activityStatusEl = document.getElementById("activityStatus");
    const activityIndicatorEl = document.getElementById("activityIndicator");
    const activityStatusClass = getStatusColor(
      data.activity_status,
      "activity"
    );
    activityStatusEl.textContent = data.activity_status;
    activityStatusEl.className = `value ${activityStatusClass}`;
    activityIndicatorEl.className = `status-indicator bg-${activityStatusClass}`;

    // Update Temperature
    const tempEl = document.getElementById("temperature");
    const tempClass = getTemperatureColor(data.temperature);
    tempEl.textContent = `${data.temperature}°C`;
    tempEl.className = `value ${tempClass}`;

    const tempFill = document.getElementById("temperatureFill");
    tempFill.style.width = `${(data.temperature / 45) * 100}%`;
    tempFill.className = `temperature-fill bg-${tempClass}`;

    // Update Timestamp
    const date = new Date(data.timestamp);
    const options = {
      year: "numeric",
      month: "2-digit",
      day: "2-digit",
      hour: "2-digit",
      minute: "2-digit",
      second: "2-digit",
      hour12: false,
    };
    const localTime = new Intl.DateTimeFormat("en-GB", options).format(date);
    document.getElementById("timestamp").textContent = localTime;
  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

/**
 * Handles the form submission for setting a new medication reminder.
 */
function handleMedicationSubmit() {
  const medicationName = document.getElementById("medicationName").value;
  const medicationTime = document.getElementById("medicationTime").value;

  if (medicationName && medicationTime) {
    const medStatusEl = document.getElementById("medStatus");
    medStatusEl.textContent = `${medicationName} at ${medicationTime}`;

    // Show toast notification
    showToast(
      `Medication "${medicationName}" has been set for ${medicationTime}`
    );

    // Clear input fields
    document.getElementById("medicationName").value = "";
    document.getElementById("medicationTime").value = "";
  }
}

/**
 * Shows a toast notification with the given message.
 */
function showToast(message) {
  const toast = document.createElement("div");
  toast.className = "toast";
  toast.textContent = message;

  const container = document.querySelector(".toast-container");
  container.appendChild(toast);

  // Remove toast after 3 seconds
  setTimeout(() => {
    toast.style.animation = "slideOut 0.3s ease-out forwards";
    setTimeout(() => {
      container.removeChild(toast);
    }, 300);
  }, 3000);
}

/**
 * Toggles the dark mode on the page.
 */
function toggleDarkMode() {
  document.body.classList.toggle("dark-mode");
  const button = document.getElementById("darkModeToggle");
  const isDark = document.body.classList.contains("dark-mode");
  button.innerHTML = isDark ? "☀️ Light Mode" : "🌙 Dark Mode";

  localStorage.setItem("darkMode", isDark ? "enabled" : "disabled");
}

document.addEventListener("DOMContentLoaded", () => {
  const darkMode = localStorage.getItem("darkMode");
  if (darkMode === "enabled") {
    document.body.classList.add("dark-mode");
    document.getElementById("darkModeToggle").innerHTML = "☀️ Light Mode";
  }
});

// Fetch data every 1 seconds
setInterval(fetchData, 1000);

// Initial fetch when page loads
window.onload = fetchData;
