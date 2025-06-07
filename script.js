// Firebase configuration

// Initialize Firebase
firebase.initializeApp(firebaseConfig);

// Reference to the database
const db = firebase.database();

// Function to update the dashboard with parking and environment data
function updateDashboard(data) {
  console.log("Raw data received:", data);
  
  // More defensive data access
  const parking = (data && data.parking_system && data.parking_system.parking) || {};
  const environment = (data && data.parking_system && data.parking_system.environment) || {};
  
  console.log("Parking data:", parking);
  console.log("Environment data:", environment);

  // Update parking status
  document.getElementById('parking-status').innerHTML = `
    <h2>🚗 Parking Status</h2>
    <p>Slot 1: ${parking.slot1 ? 'Occupied' : 'Available'}</p>
    <p>Slot 2: ${parking.slot2 ? 'Occupied' : 'Available'}</p>
    <p>Available Slots: ${parking.available || 0}</p>
    <p>Entry: ${parking.entry ? 'Open' : 'Closed'}</p>
    <p>Exit: ${parking.exit ? 'Open' : 'Closed'}</p>
  `;

  // Update alerts with more robust boolean checks
  let alerts = [];
  
  if (environment.rain === true) {
    console.log("Rain alert triggered");
    alerts.push('<div class="alert rain">🌧️ Rain Alert: Shade activated in parking area</div>');
  }
  
  if (environment.fire === true) {
    console.log("Fire alert triggered");
    alerts.push('<div class="alert fire">🔥 Fire Alert in Food Court: Evacuate immediately</div>');
  }
  
  if (environment.smog === true) {
    console.log("Smog alert triggered");
    alerts.push('<div class="alert smog">🌫️ Smog Alert in Construction Zone: Hazardous air</div>');
  }

  console.log("Generated alerts:", alerts);
  
  document.getElementById('alerts').innerHTML = `
    <h2>⚠️ Alerts</h2>
    ${alerts.length ? alerts.join('') : '<div class="alert normal">All systems normal</div>'}
  `;
}

document.querySelectorAll('.nav-link').forEach(link => {
  link.addEventListener('click', (e) => {
    e.preventDefault();
    const targetId = link.getAttribute('href').substring(1);
    
    // Hide all sections
    document.querySelectorAll('main > section').forEach(section => {
      section.style.display = 'none';
    });
    
    // Show target section
    document.getElementById(targetId).style.display = 'block';
    
    // Scroll to top
    window.scrollTo(0, 0);
  });
});
// Listen to changes in the Firebase database
db.ref('/').on('value', (snapshot) => {
  console.log("COMPLETE FIREBASE DATA:", JSON.stringify(snapshot.val(), null, 2));
  if (snapshot.exists()) {
    updateDashboard(snapshot.val());
  }
});
