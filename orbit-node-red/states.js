const closeDistance = 1;

const offState = "off";
const readyAutoState = "readyAuto";
const slowState = "slow";
const fastState = "fast";

let stateMachineMode = flow.get("stateMachineMode");
if (stateMachineMode === undefined) {
  stateMachineMode = "auto";
}
let currentState = flow.get("currentState") || readyAutoState;
let currentStateTimestamp = flow.get("currentStateTimestamp") || 0;

let timestamp = msg.payload;
let tfmini = flow.get("tfmini") || { distance: 0 }; // Ensure tfmini is an object with a distance property

function updateState(newState, timestamp) {
  currentState = newState;
  flow.set("currentStateTimestamp", timestamp);
  flow.set("currentState", currentState);
}

function getCommand(state) {
  switch (state) {
    case offState:
      return "off";
    case readyAutoState:
      return "off";
    case slowState:
      return "slow";
    case fastState:
      return "fast";
  }
}

function handleStateTransition(currentState, timeInCurrentState, distance, timestamp) {
  switch (currentState) {
    case readyAutoState:
      if (distance == closeDistance) {
        return slowState;
      }
      break;

    case slowState:
      if (timeInCurrentState >= 15000 && distance == closeDistance) {
        return fastState;
      } else if (timeInCurrentState >= 30000 && distance == 0) {
        return readyAutoState;
      }
      break;

    case fastState:
      if (timeInCurrentState >= 15000 && distance == closeDistance) {
        return slowState;
      } else if (timeInCurrentState >= 30000 && distance == 0) {
        return readyAutoState;
      }
      break;

    case offState:
      return slowState;
  }
  return currentState;
}

function updatePayload(currentState, timeInCurrentState, distance) {
  return {
    timestampDiff: timeInCurrentState,
    command: getCommand(currentState),
    distance: distance,
    mode: currentState,
  };
}

if (stateMachineMode === "auto") { // Check tfmini.distance instead of tfmini
  const timeInCurrentState = timestamp - currentStateTimestamp;
  const newState = handleStateTransition(currentState, timeInCurrentState, tfmini.distance, timestamp);

  if (newState !== currentState) {
    updateState(newState, timestamp);
  }

  msg.payload = updatePayload(newState, timeInCurrentState, tfmini.distance);
  return msg;
}

msg.payload = {
  timestampDiff: 0,
  command: getCommand(stateMachineMode),
  distance: tfmini.distance,
  mode: stateMachineMode,
};
return msg;
