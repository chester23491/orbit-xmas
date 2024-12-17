// Assume `msg.payload` contains the raw buffer from the serial port.
let data = Buffer.isBuffer(msg.payload)
  ? msg.payload
  : Buffer.from(msg.payload);

// Check if we have enough data (at least 9 bytes) and the header is correct
if (data.length >= 8 && data[0] === 0x59 && data[1] === 0x59) {
  // Parse the distance, strength, and temperature values
  let distance = data[2] + (data[3] << 8); // Combine low and high bytes for distance
  let strength = data[4] + (data[5] << 8); // Combine low and high bytes for strength
  let temperatureRaw = data[6] + (data[7] << 8); // Combine low and high bytes for temperature
  let temperature = temperatureRaw / 8 - 256; // Convert raw temperature value

  // Construct the output object
  let result = {
    distance: distance,
    strength: strength,
    temperature: temperature,
  };

  if (strength > 100 && distance < 1200) {
    flow.set("tfmini", result);
  }

  // Pass the result to the next node
  msg.payload = result;
  return msg;
} else {
  // If data is not valid, pass an error message
  msg.payload = { error: "Invalid data or header mismatch" };
  return msg;
}
