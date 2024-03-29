
<h1 align="center">Plant Tracker</h1>
<blockquote align="center">Your stomach tells you when you're hungry, so why can't your plants do the same?</blockquote>

<div align="center">
<img src="https://github.com/jlemanski1/PlantTracker/assets/12676218/45a40007-6d5d-42ed-8f95-256e7221a8a0" alt="Photo of the plant" width="350" height="auto"/>

<img src="https://github.com/jlemanski1/PlantTracker/assets/12676218/77acf460-26ff-4664-a5d9-1b0d6c114340" alt="Photo of the " width="350" height="auto"/>
</div>

<br/>

**The Plant** -  This plant gets a lot of sun causing it to be quite thirsty.

**The Problem** - The plant does not receive consistent care/watering due to several pain points related to the height the plant is mounted. Watering it on a regular schedule can lead to over/under-watering dependent on the weather, and feeling the soil moisture levels by hand requires setting up a ladder to reach the plant.

**The Solution** - The previous attempts at giving the plant more consistent water failed and so rather than follow the logical and low tech route of investing more time in researching the plant, I opted to have some fun with and create a networked plant.

<br/>

<h3 align="center">Basic Project Overview</h3>

![Basic Project Overview](https://github.com/jlemanski1/PlantTracker/assets/12676218/109f4461-0281-49e3-aeb6-36b875df3d1f)
<details>
<summary>Architecture Comments</summary>
While this could be greatly simplified by having the ESP32 send the email via the Sendgrid API cutting out Firebase/GCP entirely, I opted not to do that for several reasons:
  <ul>
  <li>The current set up allows me to extend the device battery life by having the device perform minimal actions before going back into deep sleep.</li>
  <li>I'm able to extend the functionality, fix bugs, etc., without having to climb a ladder to retrieve the device, take the system offline, and reflash it everytime I need to test/change something.</li>
  <li>Having the values stored allows me to do some data visualization in the future if I so choose.</li>
  </ul>
</details>
  
### Repository Structure
This repository servers as a (small) monorepo for all the components involved in this project. The repository is organized with separate directories for the IoT device firmware, the files for 3D printing the enclosure, and the cloud function.
```
├── CloudFunctions -> Firebase Cloud Functions
├── Tracker        -> Code for the ESP32 microcontroller
├── 3D Files       -> STL files for 3D printing the enclosure
```


## Project Cost

### Hardware
Initial Setup costs for a single plant set up is CAD 28.65$. I opted to order from closer distributors and pay a higher cost to avoid long shipping times but you can trade that off by ordering from aliexpress or in bulk to do this cheaper. You can also opt for a different board, I happened to have a few of the ones used laying around. I haven't accounted for the cost of filament or a 3D printer (or printing service).
- CAD 11.71$ - RobotShop: [DFR Robot FireBeetle ESP32 IoT Microcontroller](https://ca.robotshop.com/products/firebeetle-esp32-iot-microcontroller?variant=42413189267607)
- CAD 6.94$ - RobotShop: [DFR Robot Gravity Moisture Sensor](https://ca.robotshop.com/products/gravity-moisture-sensor-corrosion-resistant?variant=42411303370903)
- CAD ~10$ Li-On battery cell purchased from local hobby/electronics shop

### Cloud/API Fees
- Firebase / GCP have generous free tiers which will not be exceeded under normal usage of the Plant Tracker IoT device
- SendGrid has a generous free tier of 100 emails/day. With only 24 hours in a day, the current configuration will keep this project in the free tier.


## Security
### IoT Device
Secrets required by the Plant Tracker IoT device are defined in a header file. This doesn't represent industry best practices, however, it keeps the values out of git and is secure enough for comfort within the scope of this project. Since this device is only deployed in my house, if someone is able to break in, I will have bigger concerns than them decompiling the firmware and stealing my wifi password.
### Cloud Services
- Send Grid API Key stored in Google Secret Manager
- Plant Tracker IoT device is authenticated in Firebase with an email and password.
- Firebase Realtime Database only allows writes from the IoT device's user's UID, and doesn't allow any reads (except Admin SDK).
- The Cloud Function's only trigger is a new value written to the Realtime Database.
- Physical Security-wise, the plant is hung high on the ceiling far out of reach of any pet swipes or bites.

## Future Updates
As this plant is the only one with the pain points described above, I've considered this project "finished" even though it's in an MVP-esque state. The following are some features that could be added if the inspiration strikes:
- [ ] Improve documentation & polish for release to others (e.g. Terraform to set up Firebase project)
- [ ] Support multiple plants (either as standalone units or in a mesh network with a single device uploading to Firebase RTDB)
- [ ] Increase test coverage for cloud function
- [ ] Increase test coverage for ESP32 firmware

## Contributing
I have zero expectations for anyone to contribute as this is a personal project aimed at solving an issue of mine in a way that provides an opportunity to learn and have some fun along the way. That being said, in the spirit of open source, issues and pull requests are always welcome!

1.  [Fork it](https://github.com/jlemanski1/PlantTracker/fork)
2.  Create your feature branch (`git checkout -b feature/fooBar`)
3.  Commit your changes (`git commit -am 'Add some fooBar'`)
4.  Push to the branch (`git push origin feature/fooBar`)
5.  Create a new Pull Request

## License
