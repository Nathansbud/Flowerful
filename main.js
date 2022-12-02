import { analyze } from 'web-audio-beat-detector'
import * as THREE from 'three'

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
            console.log(sum)

            cube.rotation.x += 1;
            cube.rotation.y += 1;
            line.rotation.x += 1;
            line.rotation.y += 1;
        }, 60000 / tempo);
    })
    
    
    bufSrc.connect(analyzer);
    analyzer.connect(ctx.destination);
    bufSrc.start()

    bufSrc.addEventListener("ended", () => {
        console.log("vibes")
    })

}

// THREE STUFF

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
const audioListener = new THREE.AudioListener();

camera.add(audioListener);
const track = new THREE.Audio(audioListener);
scene.add(track);

// RENDER
const renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );
const geometry = new THREE.BoxGeometry( 1, 1, 1 );
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });

const cube = new THREE.Mesh( geometry, material );

const edges = new THREE.EdgesGeometry( geometry );
const line = new THREE.LineSegments( edges, new THREE.LineBasicMaterial( { color: 0xffffff } ) );
scene.add( line );

scene.add(cube);

camera.position.z = 5;
function animate() {
    requestAnimationFrame(animate);
    renderer.render( scene, camera );
};
// END RENDER

camera.add(audioListener);
// const trackTest = new THREE.Audio()

animate();
