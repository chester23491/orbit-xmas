const closeDistance = 50;

const offState = "off";
const pauseState = "pause";
const normalState = "normal";
const salesState = "sales";

let stateMachineEnabled = flow.get("stateMachineEnabled") || false;
let currentState =
  flow.get("currentState") || (stateMachineEnabled ? pauseState : offState);
let currentStateTimestamp = flow.get("currentStateTimestamp") || 0;

let timestamp = msg.payload;
let tfmini = flow.get("tfmini") || 0;

function updateState(newState, timestamp) {
  currentState = newState;
  flow.set("currentStateTimestamp", timestamp);
  flow.set("currentState", currentState);
}

if (stateMachineEnabled && tfmini !== 0) {
  const timeInCurrentState = timestamp - currentStateTimestamp;

  switch (currentState) {
    case pauseState:
      if (timeInCurrentState >= 15000 && tfmini.distance < closeDistance) {
        updateState(normalState, timestamp);
      } else if (
        timeInCurrentState >= 90000 &&
        tfmini.distance > closeDistance
      ) {
        updateState(normalState, timestamp);
      }
      break;

    case normalState:
      if (timeInCurrentState >= 15000 && tfmini.distance < closeDistance) {
        updateState(salesState, timestamp);
      } else if (
        timeInCurrentState >= 60000 &&
        tfmini.distance > closeDistance
      ) {
        updateState(pauseState, timestamp);
      } else if (
        timeInCurrentState >= 30000 &&
        tfmini.distance > closeDistance
      ) {
        updateState(pauseState, timestamp);
      }
      break;

    case salesState:
      if (timeInCurrentState >= 60000 && tfmini.distance > closeDistance) {
        updateState(pauseState, timestamp);
      }
      break;
  }

  msg.payload = {
    timestampDiff: timeInCurrentState,
    currentState: currentState,
    distance: tfmini.distance,
  };
  return msg;
}

msg.payload = {
  timestampDiff: 0,
  currentState: offState,
  distance: tfmini.distance,
};
return msg;
