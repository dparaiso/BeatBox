"use strict";

var socket = io.connect();
$(document).ready(() => {
    $('#btnVolUp').click(() => {
        var volLvl = Number($('#volLvl').val());
        if(volLvl < 100-5) {
            $('#volLvl').val(volLvl+5);
        }
        else {
            $('#volLvl').val(100);
        }
        sendCommand("set_vol " + $('#volLvl').val());
    });
    $('#btnVolDown').click(() => {
        var volLvl = Number($('#volLvl').val());
        if(volLvl > 5) {
            $('#volLvl').val(volLvl-5);
        }
        else {
            $('#volLvl').val(0);
        }
        sendCommand("set_vol " + $('#volLvl').val());
    })
});

const sendCommand = (msg) => {
    socket.emit('beatBoxCommand', msg);
}