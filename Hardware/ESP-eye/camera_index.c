#include "camera_index.h"

const char index_ov2640_html[] PROGMEM = R"(
<!doctype html>
  <html>
  <head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 OV2460</title>
<style>
body {
  font-family: Arial,Helvetica,sans-serif;
  background: #181818;
  color: #EFEFEF;
  font-size: 16px
}
h2 {
  font-size: 18px
}
section.main {
  display: flex
}
#menu,section.main {
  flex-direction: column
}
#menu {
  display: none;
  flex-wrap: nowrap;
  min-width: 340px;
  background: #363636;
  padding: 8px;
  border-radius: 4px;
  margin-top: -10px;
  margin-right: 10px;
}
#content {
  display: flex;
  flex-wrap: wrap;
  align-items: stretch
}
figure {
  padding: 0px;
  margin: 0;
  -webkit-margin-before: 0;
  margin-block-start: 0;
  -webkit-margin-after: 0;
  margin-block-end: 0;
  -webkit-margin-start: 0;
  margin-inline-start: 0;
  -webkit-margin-end: 0;
  margin-inline-end: 0
}
figure img {
  display: block;
  width: 100%;
  height: auto;
  border-radius: 4px;
  margin-top: 8px;
}
@media (min-width: 800px) and (orientation:landscape) {
  #content {
    display:flex;
    flex-wrap: nowrap;
    align-items: stretch
  }
  figure img {
    display: block;
    max-width: 100%;
    max-height: calc(100vh - 40px);
    width: auto;
    height: auto
  }
  figure {
    padding: 0 0 0 0px;
    margin: 0;
    -webkit-margin-before: 0;
    margin-block-start: 0;
    -webkit-margin-after: 0;
    margin-block-end: 0;
    -webkit-margin-start: 0;
    margin-inline-start: 0;
    -webkit-margin-end: 0;
    margin-inline-end: 0
  }
}
section#buttons {
  display: flex;
  flex-wrap: nowrap;
  justify-content: space-between
}
#nav-toggle {
  cursor: pointer;
  display: block
}
#nav-toggle-cb {
  outline: 0;
  opacity: 0;
  width: 0;
  height: 0
}
#nav-toggle-cb:checked+#menu {
  display: flex
}
.input-group {
  display: flex;
  flex-wrap: nowrap;
  line-height: 22px;
  margin: 5px 0
}
.input-group>label {
  display: inline-block;
  padding-right: 10px;
  min-width: 47%
}
.input-group input,.input-group select {
  flex-grow: 1
}
.range-max,.range-min {
  display: inline-block;
  padding: 0 5px
}
button, .button {
  display: block;
  margin: 5px;
  padding: 0 12px;
  border: 0;
  line-height: 28px;
  cursor: pointer;
  color: #fff;
  background: #ff3034;
  border-radius: 5px;
  font-size: 16px;
  outline: 0
}
.save {
  position: absolute;
  right: 25px;
  top: 0px;
  height: 16px;
  line-height: 16px;
  padding: 0 4px;
  text-decoration: none;
  cursor: pointer
}
button:hover {
  background: #ff494d
}
button:active {
  background: #f21c21
}
button.disabled {
  cursor: default;
  background: #a0a0a0
}
input[type=range] {
  -webkit-appearance: none;
  width: 100%;
  height: 22px;
  background: #363636;
  cursor: pointer;
  margin: 0
}
input[type=range]:focus {
  outline: 0
}
input[type=range]::-webkit-slider-runnable-track {
  width: 100%;
  height: 2px;
  cursor: pointer;
  background: #EFEFEF;
  border-radius: 0;
  border: 0 solid #EFEFEF
}
input[type=range]::-webkit-slider-thumb {
  border: 1px solid rgba(0,0,30,0);
  height: 22px;
  width: 22px;
  border-radius: 50px;
  background: #ff3034;
  cursor: pointer;
  -webkit-appearance: none;
  margin-top: -11.5px
}
input[type=range]:focus::-webkit-slider-runnable-track {
  background: #EFEFEF
}
input[type=range]::-moz-range-track {
  width: 100%;
  height: 2px;
  cursor: pointer;
  background: #EFEFEF;
  border-radius: 0;
  border: 0 solid #EFEFEF
}
input[type=range]::-moz-range-thumb {
  border: 1px solid rgba(0,0,30,0);
  height: 22px;
  width: 22px;
  border-radius: 50px;
  background: #ff3034;
  cursor: pointer
}
input[type=range]::-ms-track {
  width: 100%;
  height: 2px;
  cursor: pointer;
  background: 0 0;
  border-color: transparent;
  color: transparent
}
input[type=range]::-ms-fill-lower {
  background: #EFEFEF;
  border: 0 solid #EFEFEF;
  border-radius: 0
}
input[type=range]::-ms-fill-upper {
  background: #EFEFEF;
  border: 0 solid #EFEFEF;
  border-radius: 0
}
input[type=range]::-ms-thumb {
  border: 1px solid rgba(0,0,30,0);
  height: 22px;
  width: 22px;
  border-radius: 50px;
  background: #ff3034;
  cursor: pointer;
  height: 2px
}
input[type=range]:focus::-ms-fill-lower {
  background: #EFEFEF
}
input[type=range]:focus::-ms-fill-upper {
  background: #363636
}
.switch {
  display: block;
  position: relative;
  line-height: 22px;
  font-size: 16px;
  height: 22px
}
.switch input {
  outline: 0;
  opacity: 0;
  width: 0;
  height: 0
}
.slider {
  width: 50px;
  height: 22px;
  border-radius: 22px;
  cursor: pointer;
  background-color: grey
}
.slider,.slider:before {
  display: inline-block;
  transition: .4s
}
.slider:before {
  position: relative;
  content: "";
  border-radius: 50%;
  height: 16px;
  width: 16px;
  left: 4px;
  top: 3px;
  background-color: #fff
}
input:checked+.slider {
  background-color: #ff3034
}
input:checked+.slider:before {
  -webkit-transform: translateX(26px);
  transform: translateX(26px)
}
select {
  border: 1px solid #363636;
  font-size: 14px;
  height: 22px;
  outline: 0;
  border-radius: 5px
}
.image-container {
  position: relative;
  min-width: 160px
}
.close {
  position: absolute;
  right: 5px;
  top: 5px;
  background: #ff3034;
  width: 16px;
  height: 16px;
  border-radius: 100px;
  color: #fff;
  text-align: center;
  line-height: 18px;
  cursor: pointer
}
.hidden {
  display: none
}
input[type=text] {
  border: 1px solid #363636;
  font-size: 14px;
  height: 20px;
  margin: 1px;
  outline: 0;
  border-radius: 5px
}
.inline-button {
  line-height: 20px;
  margin: 2px;
  padding: 1px 4px 2px 4px;
}
label.toggle-section-label {
  cursor: pointer;
  display: block
}
input.toggle-section-button {
  outline: 0;
  opacity: 0;
  width: 0;
  height: 0
}
input.toggle-section-button:checked+section.toggle-section {
  display: none
}
</style>
</head>
<body>
  <section class="main">
    <div id="logo">
      <label for="nav-toggle-cb" id="nav-toggle">&#9776;&nbsp;&nbsp;Toggle OV2640 settings (800x600)</label>
    </div>
    <div id="content">
      <div id="sidebar">
        <input type="checkbox" id="nav-toggle-cb" checked="checked">
        <nav id="menu">

          <div class="input-group" id="quality-group">
            <label for="quality">Quality</label>
            <div class="range-min">4</div>
            <input type="range" id="quality" min="4" max="63" value="10" class="default-action">
            <div class="range-max">63</div>
          </div> 

          <div class="input-group" id="hmirror-group">
            <label for="hmirror">H-Mirror</label>
            <div class="switch">
            <input id="hmirror" type="checkbox" class="default-action" checked="checked">
            <label class="slider" for="hmirror"></label>
            </div>
          </div>

          <div class="input-group" id="vflip-group">
            <label for="vflip">V-Flip</label>
            <div class="switch">
            <input id="vflip" type="checkbox" class="default-action" checked="checked">
            <label class="slider" for="vflip"></label>
            </div>
          </div>

          <div class="input-group" id="ir-led-group">
            <label for="ir_led_intensity">IR LED</label>
            <div class="switch">
            <input id="ir_led_intensity" type="checkbox" class="default-action" checked="checked">
            <label class="slider" for="ir_led_intensity"></label>
            </div>
          </div>

          <div class="input-group" id="led-group">
            <label for="led_intensity">LED Intensity</label>
            <div class="range-min">0</div>
            <input type="range" id="led_intensity" min="0" max="255" value="0" class="default-action">
            <div class="range-max">255</div>
          </div>

          <section id="buttons">
            <button id="tv-on-off">TV On/Off</button>
            <button id="get-still">Get Still</button>
            <button id="toggle-stream">Start Stream</button>
          </section>
      </nav>
    </div>
    <figure>
      <div id="stream-container" class="image-container hidden">
        <img id="stream" src="" crossorigin>
      </div>
    </figure>
  </div>
</section>

<script>
document.addEventListener('DOMContentLoaded', function (event) {
  var baseHost = document.location.origin
  var streamUrl = baseHost + ':81'

  function fetchUrl(url, cb){
    fetch(url)
      .then(function (response) {
        if (response.status !== 200) {
          cb(response.status, response.statusText);
        } else {
          response.text().then(function(data){
            cb(200, data);
          }).catch(function(err) {
            cb(-1, err);
          });
        }
      })
      .catch(function(err) {
        cb(-1, err);
      });
  } 

  const hide = el => {
    el.classList.add('hidden')
  }
  const show = el => {
    el.classList.remove('hidden')
  }

  const disable = el => {
    el.classList.add('disabled')
    el.disabled = true
  }

  const enable = el => {
    el.classList.remove('disabled')
    el.disabled = false
  }

  const updateValue = (el, value, updateRemote) => {
    updateRemote = updateRemote == null ? true : updateRemote
    let initialValue
    if (el.type === 'checkbox') {
      initialValue = el.checked
      value = !!value
      el.checked = value
    } else {
      initialValue = el.value
      el.value = value
    }

    if (updateRemote && initialValue !== value) {
      updateConfig(el);
    } else if(!updateRemote){
      if(el.id == "led_intensity"){
        value > -1 ? show(ledGroup) : hide(ledGroup)
      }
    }
  }

  function updateConfig (el) {
    let value
    switch (el.type) {
      case 'checkbox':
        value = el.checked ? 1 : 0
        break
      case 'range':
      case 'select-one':
        value = el.value
        break
      case 'button':
      case 'submit':
        value = '1'
        break
      default:
        return
    }

    const query = `${baseHost}/control?var=${el.id}&val=${value}`

    fetch(query)
      .then(response => {
        console.log(`request to ${query} finished, status: ${response.status}`)
      })
  }

  document
    .querySelectorAll('.close')
    .forEach(el => {
      el.onclick = () => {
        hide(el.parentNode)
      }
    })

  // read initial values
  fetch(`${baseHost}/status`)
    .then(function (response) {
      return response.json()
    })
    .then(function (state) {
      document
        .querySelectorAll('.default-action')
        .forEach(el => {
          updateValue(el, state[el.id], false)
        })      
    })

  const view = document.getElementById('stream')
  const viewContainer = document.getElementById('stream-container')
  const tvOnOffButton = document.getElementById('tv-on-off')
  const stillButton = document.getElementById('get-still')  
  const streamButton = document.getElementById('toggle-stream')
  const ledGroup = document.getElementById('led-group')

  const irRemoteCtrlSend = () => {
    const query = `${baseHost}/control?var=ir_ctrl&val=2`

    fetch(query)
      .then(response => {
        console.log(`request to ${query} finished, status: ${response.status}`)
      })
  }

  const stopStream = () => {
    window.stop();
    streamButton.innerHTML = 'Start Stream'
  }

  const startStream = () => {
    view.src = `${streamUrl}/stream`
    show(viewContainer)
    streamButton.innerHTML = 'Stop Stream'
  }  

  // Attach actions to buttons
  tvOnOffButton.onclick = () => {
    stopStream()    
  }

  stillButton.onclick = () => {
    stopStream()
    view.src = `${baseHost}/capture?_cb=${Date.now()}`
    show(viewContainer)
  }

  streamButton.onclick = () => {
    const streamEnabled = streamButton.innerHTML === 'Stop Stream'
    if (streamEnabled) {
      stopStream()
    } else {
      startStream()
    }
  }

 
  // Attach default on change action
  document
    .querySelectorAll('.default-action')
    .forEach(el => {
      el.onchange = () => updateConfig(el)
    })
})
</script>
</body>
</html>

)";
