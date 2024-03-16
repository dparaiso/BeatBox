"use strict";

var socket = io.connect();

$(document).ready(() => {
    //initial values
    $('#current-mode').val("No Drum");
    $('#volume').val(80);
    $('#bpm').val(120);

    $('#btnNoDrum').click(() => {
        sendCommand("set_beat 0");
        $('#current-mode').val("No Drum");
    });
    $('#btnStandardDrum').click(() => {
        sendCommand("set_beat 1");
        $('#current-mode').val("Standard Rock Beat");
    });
    $('#btnOtherDrum').click(() => {
        sendCommand("set_beat 2");
        $('#current-mode').val("Other Beat");
    });

    $('#btnVolUp').click(() => {
        var volume = Number($('#volume').val());
        if(volume < 100-5) {
            $('#volume').val(volume+5);
        }
        else {
            $('#volume').val(100);
        }
        sendCommand("set_vol " + $('#volume').val());
    });
    $('#btnVolDown').click(() => {
        var volume = Number($('#volume').val());
        if(volume > 5) {
            $('#volume').val(volume-5);
        }
        else {
            $('#volume').val(0);
        }
        sendCommand("set_vol " + $('#volume').val());
    });

    $('#btnBpmUp').click(() => {
        var bpm = Number($('#bpm').val());
        if(bpm < 300-5) {
            $('#bpm').val(bpm+5);
        }
        else {
            $('#bpm').val(300);
        }
        sendCommand("set_bpm " + $('#bpm').val());
    });
    $('#btnBpmDown').click(() => {
        var bpm = Number($('#bpm').val());
        if(bpm > 45) {
            $('#bpm').val(bpm-5);
        }
        else {
            $('#bpm').val(40);
        }
        sendCommand("set_bpm " + $('#bpm').val());
    });

    $('#stopProgram').click(() => {
        sendCommand("stop");
    });
});

const sendCommand = (msg) => {
    socket.emit('beatBoxCommand', msg + '\0');
}