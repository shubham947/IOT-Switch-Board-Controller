const switchURL = "/switch";
const getSwitchStatesURL = "/getSwitchStates";

const deviceIds = ['R1D1','R1D2','R1D3','R1D4'];

// Runs automatically on document load
window.addEventListener('load', (event) => {
    initializeSwitches();
    // Reload switch states every 10 sec
    setInterval(() => {
        initializeSwitches();
    }, 10000);
});

// Get Initial states of Device switches
function initializeSwitches() {
    getSwitchStates(getSwitchStatesURL, {deviceIds}).then((res)=> {
        deviceIds.forEach(id => {
            let checkBox = document.getElementById('status' + id);
            checkBox.checked = res[id];
        });
    }).catch((err)=> {
        console.log(err);
    });
}

// Set state of a device switch based on deviceId
function setDeviceState(room, device) {
    // readSettings();
    let deviceId = room + device;
    let checkBox = document.getElementById('status' + deviceId);
    let data = {'deviceId': deviceId, 'state': checkBox.checked};

    let deviceState = checkBox.checked ? 'ON' : 'OFF';

    //Sending Post request to turn that device on/off
    postDataToUrl(switchURL,data).then((success)=> {
        if (success) {
            console.log("Device " + deviceId + " is turned " + deviceState);
        } else {
            // Switching to previous state on failure
            checkBox.checked = !checkBox.checked;
            console.log("Could not turn device " + deviceId + " " + deviceState);
        }
    }).catch((err)=> {
        console.log(err);
    });
    
    //TODO: Update settings.json for current device
}

// POST data to URL
function postDataToUrl(url, jsonData) {
    return new Promise((response, reject) => {
        let xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onreadystatechange = function () {
            if (xhr.readyState === 4 && xhr.status === 200) {
                let res = xhr.statusText;
                res === "OK" ? response(true): response(false);
            }
        };
        xhr.onerror = function () {
            reject(xhr);
        };
        xhr.send(JSON.stringify(jsonData));
    });
}

function getSwitchStates(url, deviceIds) {
    return new Promise((response, reject) => {
        let xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onreadystatechange = function () {
            if (xhr.readyState === 4 && xhr.status === 200) {
                let res = JSON.parse(xhr.responseText);
                response(res);
            }
        };
        xhr.onerror = function () {
            reject(xhr);
        };
        xhr.send(JSON.stringify(deviceIds));
    });
}