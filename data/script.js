function setDeviceState(room, device) {
    // readSettings();
    var id = 'status' + room + device;
    var checkBox = document.getElementById(id);

    if (checkBox.checked == true) {
        //Run script to turn that device on
        //Update settings.json for current device
        console.log(id + " is on");
    }
    else {
        //Run script to turn that device off
        //Update settings.json for current device
        console.log(id + " is off");
    }
}