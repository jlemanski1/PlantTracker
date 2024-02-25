const {onValueCreated} = require("firebase-functions/v2/database");
const admin = require("firebase-admin");
require("firebase-functions/logger/compat");
const {defineSecret} = require("firebase-functions/params");
const sgMail = require("@sendgrid/mail");

// Init to access realtime database & remote config
admin.initializeApp();
const sendgridKey = defineSecret("SENDGRID_KEY");

/**
 * Triggered when a value is written to the realtime database.
 * Reads the latest value, and emails a notice to water the plant
 * if in the specified range.
 */
exports.handleMoistureValue = onValueCreated({ref: "/*", secrets: [sendgridKey]}, async (event) => {
  // Read value written to RTDB
  const moisture = event.data.val().moisture;
  console.log(`Value written to RTDB->moisture: ${moisture}`);


  // Get moisture sensor calibration values from remtoe config
  const {airValue, waterValue} = await getMoistureSensorConfig();
  console.log(`Soil sensor values fetched from RemoteConfig->{air: ${airValue}, water: ${waterValue}}`);


  // Plant needs water if the soil is within 10% of the dry value
  const needsWater = moisture >= airValue * 0.9;
  if (needsWater) {
    sgMail.setApiKey(sendgridKey.value());
    sendEmail(moisture);
  }
});


/**
 * Fetches the remote config parameters and returns the ones related to the soil moisture sensor.
 * @return {{airValue: int, waterValue: int}} Map of the edge values used for configuring the sensor
 */
async function getMoistureSensorConfig() {
  const config = admin.remoteConfig();
  const template = await config.getTemplate();

  const moistureParams = template.parameterGroups["Moisture Sensor"].parameters;
  const airValue = moistureParams["air_value"].defaultValue.value;
  const waterValue = moistureParams["water_value"].defaultValue.value;

  return {airValue, waterValue};
}


/**
 * Send an email through the sendgrid api to notify that the plant needs watering
 * @param {int} moisture
 */
function sendEmail(moisture) {
  const msg = {
    to: "jlemanski1@gmail.com",
    from: "jlemanski1@gmail.com",
    subject: "Time to water the plant! - PlantTracker",
    text: `moisture level: ${moisture}`,
  };

  sgMail
      .send(msg)
      .then(() => console.log(`Email Sent with moisture level: ${moisture}`))
      .catch((error) => console.error(error));
}
