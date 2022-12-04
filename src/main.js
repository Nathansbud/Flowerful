import { analyze } from 'web-audio-beat-detector'

import * as Testing from './draw.js'

const ctx = new AudioContext();
const bufSrc = ctx.createBufferSource();
const analyzer = ctx.createAnalyser();

analyzer.fftSize = 256;
analyzer.minDecibels = -90;
analyzer.maxDecibels = -10;
analyzer.smoothingTimeConstant = 0.85;

const fileSelector = document.getElementById("track-selector");

fileSelector.addEventListener('change', loadAudio, false);

function loadAudio() { 
    const reader = new FileReader();
    reader.onload = (e) => {
        ctx.decodeAudioData(e.target.result)
            .then(doMagic)
            // .catch((_) => console.error(`Failed to load ${this.files[0].name} as mp3`));
    }

    reader.readAsArrayBuffer(this.files[0])
}

let trackTempo;
function doMagic(v) {
    bufSrc.buffer = v;
    
    // bufSrc.buffer.sampleRate => samples / second
    console.log(`Total Samples: ${bufSrc.buffer.length}`)    
    
    // const C0 = bufSrc.buffer.getChannelData(0);
    // const C1 = bufSrc.buffer.getChannelData(1);
    
    const dataArrayAlt = new Uint8Array(analyzer.frequencyBinCount);

    analyze(v).then((tempo) => {
        trackTempo = tempo;
        setInterval(() => {  
            // get current sample based on sampleRate & currentTime
            // const sampleIdx = Math.floor(ctx.currentTime * bufSrc.buffer.sampleRate);
            // const currentSample0 = C0[sampleIdx]; 
            // const currentSample1 = C1[sampleIdx]; 
            
            // ATTEMPT 1: Get intensity via frequency data
            analyzer.getByteFrequencyData(dataArrayAlt);
            let sum = 0;
            for(let i = 0; i < analyzer.frequencyBinCount; i++) {
                sum += 3 * dataArrayAlt[i];
            }
        }, 60000 / tempo);
    })
        
    bufSrc.connect(analyzer);
    analyzer.connect(ctx.destination);
    bufSrc.start()

    bufSrc.addEventListener("ended", () => {
        console.log("vibes")
    })
}
