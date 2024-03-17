"use strict";

var replyTimeout;
var socket = io.connect();
var err = false;

const showError = () => {
    replyTimeout = setTimeout(()  => {
        if(!err) {
            $('#error').show();
            $('#errorMsg').text("No response from NodeJS server");
        }
        err =true;
    }, 1000);
};

$(document).ready(() => {
    //initial values
    $('#current-mode').text("No Drum");
    $('#volume').val(80);
    $('#bpm').val(120);
    $('#error').hide();

    setInterval(() => {
        sendCommand('info');
        showError();
    }, 500);

    $('#btnNoDrum').click(() => {
        sendCommand("set_beat 0");
    });
    $('#btnStandardDrum').click(() => {
        sendCommand("set_beat 1");
    });
    $('#btnOtherDrum').click(() => {
        sendCommand("set_beat 2");
    });

    $('#btnVolUp').click(() => {
        var volume = Number($('#volume').val());
        if(volume < 100-5) {
            volume = volume + 5;
        }
        else {
            volume = 100;
        }
        sendCommand("set_vol " + volume);
    });
    $('#btnVolDown').click(() => {
        var volume = Number($('#volume').val());
        if(volume > 5) {
            volume = volume-5;
        }
        else {
            volume = 0;
        }
        sendCommand("set_vol " + volume);
    });

    $('#btnBpmUp').click(() => {
        var bpm = Number($('#bpm').val());
        if(bpm < 300-5) {
            bpm = bpm+5;
        }
        else {
            bpm = 300;
        }
        sendCommand("set_bpm " + bpm);
    });
    $('#btnBpmDown').click(() => {
        var bpm = Number($('#bpm').val());
        if(bpm > 45) {
            bpm = bpm-5;
        }
        else {
           bpm = 40;
        }
        sendCommand("set_bpm " + bpm);
    });

    $('#btnPlaySnare').click(() => {
        sendCommand("play_sound 0");
    });
    $('#btnPlayHiHat').click(() => {
        sendCommand("play_sound 1");
    });
    $('#btnPlayBass').click(() => {
        sendCommand("play_sound 2");
    });

    $('#stopProgram').click(() => {
        sendCommand("stop");
    });

    socket.on("beatBoxCommandReply", (reply) => {
        clearTimeout(replyTimeout);
        err = false;
        console.log(reply);
        if(reply == "udpServerError") {
            err = true;
            $('#error').show();
            $('#errorMsg').text("No response from C UDP server");
        }
        else {
            $('#error').hide();
        }

        reply = reply.split(" ");
        if(reply[0] == "info:") {
            $('#volume').val(reply[1]);
            $('#bpm').val(reply[2]);
            switch(reply[3]) {
                case "0":
                    $('#current-mode').text("No Drum");
                    break;
                case "1": 
                    $('#current-mode').text("Standard Rock Beat");
                    break;
                case "2": 
                    $('#current-mode').text("Other Beat");
                    break;
            }
            $('#deviceUptime').text(reply[6] + ":" + reply[5] + ":" + reply[4] + " (H:M:S)");
        }
    });
});

const sendCommand = (msg) => {
    socket.emit('beatBoxCommand', msg + '\0');
}