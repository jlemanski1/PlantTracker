const { onValueCreated } = require("firebase-functions/v2/database");
const admin = require("firebase-admin");
require("firebase-functions/logger/compat");

// Init to access realtime database & remote config
admin.initializeApp();


/**
 * Triggered when a value is written to the realtime database.
 * Reads the latest value, and emails a notice to water the plant 
 * if in the specified range.
 */
exports.handleMoistureValue = onValueCreated("/*", async (event) => {
    // Read value written to RTDB
    const moisture = event.data.val().moisture;
    console.log(`Value written to RTDB->moisture: ${moisture}`);

    const { airValue, waterValue } = await getMoistureSensorConfig();
    console.log(`Soil sensor values fetched from RemoteConfig->{air: ${airValue}, water: ${waterValue}}`);


    // If value is "dry", send out email
    const needsWater = false;
    if (needsWater) {
        sendEmail(moisture);
    }

});


/**
 * Fetches the remote config parameters and returns the ones related to the soil moisture sensor.
 * @returns {{airValue: int, waterValue: int}} Map of the edge values used for configuring the sensor
 */
async function getMoistureSensorConfig() {
    const config = admin.remoteConfig();
    const template = await config.getTemplate();

    const moistureParams = template.parameterGroups["Moisture Sensor"].parameters;
    const airValue = moistureParams["air_value"].defaultValue.value;
    const waterValue = moistureParams["water_value"].defaultValue.value;

    return { airValue, waterValue };
}


/**
 * Send an email through the sendgrid api to notify that the plant needs watering
 * @param {int} moisture 
 */
function sendEmail(moisture) {
    const sgMail = require('@sendgrid/mail');
    sgMail.setApiKey(functions.config().sendgrid.key);
    const msg = {
        to: 'jlemanski1@gmail.com',
        from: 'jlemanski1@gmail.com',
        subject: 'Time to water the plant! - PlantTracker',
        text: `moisture level: ${moisture}`,
    };

    sgMail
        .send(msg)
        .then(() => console.log(`Email Sent with moisture level: ${moisture}`))
        .catch((error) => console.error(error));
}
