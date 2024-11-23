function getStatusColor(status, type) {
    if (!status) {
      return "gray";
    }
  status = status.toLowerCase();
  switch (type) {
    case "fall":
      // Check for specific fall detections
      if (
        status === "severe fall detected" ||
        status === "moderate fall detected" ||
        status === "minor fall detected"
      ) {
        return "danger";
      }
      return "safe";
    case "activity":
      const activityColors = {
        walking: "warning",
        running: "danger",
        resting: "safe",
        default: "safe",
      };
      return activityColors[status] || activityColors.default;
    case "medication":
      return status === "MED_CONFIRM"
        ? "safe"
        : status === "MED_CANCEL"
        ? "danger"
        : "warning";
    case "panic":
      return status === "HELP: REQUESTED"
        ? "danger"
        : status === "false" || status === "HELP: CANCELLED"
        ? "safe"
        : "warning";
    default:
      return status === "safe"
        ? "safe"
        : status === "warning"
        ? "warning"
        : "danger";
  }
}

function getTemperatureColor(temp) {
  return temp >= 35 ? "danger" : temp >= 39 ? "5" : "safe";
}

async function fetchData() {
  try {
    const response = await fetch("/api/sensordata");
    const data = await response.json();

    // Update Fall Status
    const fallStatusEl = document.getElementById("fallStatus");
    const fallIndicatorEl = document.getElementById("fallIndicator");

    const fallStatuses = [
      "severe fall detected",
      "moderate fall detected",
      "minor fall detected",
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
      fallStatusEl.textContent = "UNKNOWN STATUS";
      fallStatusEl.className = "value warning";
      fallIndicatorEl.className = "status-indicator bg-warning";
    }

    // Update Medication Status
    const medStatusEl = document.getElementById("medStatus");
    const medStatusClass = getStatusColor(data.med_status, "med confirmed");
    medStatusEl.textContent = data.med_status;
    medStatusEl.className = `value ${medStatusClass}`;
    document.getElementById(
      "medIndicator"
    ).className = `status-indicator bg-${medStatusClass}`;

    // Update Panic Status
    const panicStatusEl = document.getElementById("panicStatus");
    const panicStatusClass = getStatusColor(data.panic_status, "help");
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

// Fetch data every 2 seconds
setInterval(fetchData, 2000);

// Initial fetch when page loads
window.onload = fetchData;
