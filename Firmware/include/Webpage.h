#include <Arduino.h>

const char Webpage[] = R"(
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
	<title>Control</title>

	<style>
		body {
			display: flex;
			flex-direction: column;
			align-items: center;
			font-family: Arial, sans-serif;
			background-color: #292929;
			color: #e0e0e0;
			margin: 0;
			padding: 10px;
			height: 100vh;
			overflow: hidden;
		}

		h1 {
			font-family: 'Arial', sans-serif;
			font-size: 2rem;
			font-weight: 700;
			text-transform: uppercase;
			color: #5aa6f3;
			letter-spacing: 4px;
			text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);
			line-height: 1.2;
			margin-bottom: 20px;
			transition: all 0.3s ease-in-out;
		}

		h1:hover {
			color: #943ce7;
			text-shadow: 2px 2px 8px rgba(0, 0, 0, 0.5);
		}

		#refresh-button {
			position: absolute;
			top: 40px;
			right: 30px;
			padding: 1px 1px;
			font-size: 10px;
			background-color: #3b3b5c;
			color: white;
			border: none;
			cursor: pointer;
			border-radius: 5px;
		}

		#refresh-button:hover {
			background-color: #6b6b9e;
		}

		/* Responsive black screen with 16:9 aspect ratio */
		.future-screen {
			width: 90vw;
			max-width: 600px;
			aspect-ratio: 16 / 9;
			background-color: #3b3b5cc5;
			/* Darker background */
			border: 2px solid #333;
			margin-bottom: 20px;
		}

		/* Slider styles */
		.slider-container {
			display: flex;
			align-items: center;
			gap: 8px;
			margin: 5px 0;
			width: 90%;
			background-color: #2c2c3e;
			padding: 12px;
			border-radius: 10px;
			border: 2px solid #1e1e2f;
			box-shadow: 0 2px 8px rgba(0, 0, 0, 0.4);
		}

		.slider-container label {
			width: 70px;
			text-align: right;
			color: #d1d1d1;
			font-size: 1.1em;
			font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
			font-weight: 600;
		}

		/* Styling for sliders */
		.slider {
			width: 100%;
			max-width: 220px;
			height: 8px;
			background: #3b3b5c;
			outline: none;
			opacity: 0.9;
			transition: opacity 0.2s;
			border-radius: 5px;
			border: 3px solid #3b3b5c;
			box-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
			-webkit-appearance: none;
			appearance: none;
		}

		.slider:hover {
			opacity: 1;
		}

		/* Styling for the slider thumb */
		.slider::-webkit-slider-thumb {
			width: 35px;
			height: 18px;
			background: linear-gradient(135deg, #26a5fa, #fa03ee);
			border-radius: 8px;
			box-shadow: 0 4px 10px rgba(0, 0, 0, 0.4);
			cursor: pointer;
			-webkit-appearance: none;
		}

		.slider::-moz-range-thumb {
			width: 35px;
			height: 18px;
			background: linear-gradient(135deg, #f83333, #e74c3c);
			border-radius: 8px;
			box-shadow: 0 4px 10px rgba(0, 0, 0, 0.4);
			cursor: pointer;
		}

		/* Additional browser compatibility */
		.slider::-ms-thumb {
			width: 35px;
			height: 18px;
			background: linear-gradient(135deg, #f83333, #e74c3c);
			border-radius: 8px;
			box-shadow: 0 4px 10px rgba(0, 0, 0, 0.4);
			cursor: pointer;
		}

		@media screen and (max-width: 480px) {
			.slider {
				max-width: 250px;
			}
		}

		/* Toggle switch styles */
		.toggle {
			position: relative;
			width: 55px;
			height: 25px;
			background-color: #3b3b5c;
			border-radius: 15px;
			cursor: pointer;
			transition: background-color 0.3s;
			box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);
		}

		.toggle .switch {
			position: absolute;
			top: 2px;
			left: 2px;
			width: 21px;
			height: 21px;
			background-color: #252525;
			border-radius: 50%;
			transition: all 0.3s;
		}

		.toggle.active {
			background: linear-gradient(135deg, #26a5fa, #fa03ee);
		}

		.toggle.active .switch {
			left: 30px;
			background-color: #252525;
		}

		/* Joystick styles */
		.joystick {
			display: flex;
			align-items: center;
			justify-content: center;
			background-color: #3b3b5c;
			/* Darker grey */
			width: 90px;
			height: 90px;
			border-radius: 50%;
			position: relative;
			box-shadow: 0 4px 10px rgba(0, 0, 0, 0.3);
		}

		.joystick .knob {
			width: 25px;
			height: 25px;
			background-color: #ff4a4a;
			/* Red for visual contrast */
			border-radius: 50%;
			position: absolute;
			top: 32.5px;
			left: 32.5px;
			transition: transform 0.1s ease-out;
		}

		/* Layout sections */
		.control-section {
			display: flex;
			flex-direction: column;
			align-items: center;
			width: 100%;
			max-width: 600px;
			gap: 15px;
		}

		.controls {
			display: flex;
			flex-wrap: wrap;
			justify-content: center;
			width: 100%;
			gap: 10px;
		}

		.toggle-controls {
			display: flex;
			justify-content: center;
			gap: 25px;
			margin-top: 10px;
		}

		.joystick-screen {
			display: flex;
			justify-content: space-around;
			width: 80%;
			position: absolute;
			bottom: 50px;
		}

		/* Portrait Mode */
		@media only screen and (orientation: portrait) {
			body {
				display: flex;
				flex-direction: column;
				align-items: center;
				justify-content: flex-start;
				height: 100vh;
				margin: 0;
				background-color: #121212;
			}

			.control-section {
				margin-top: 30px;
			}

			.joystick-controls {
				display: flex;
				justify-content: space-between;
				width: 70%;
				position: fixed;
				bottom: 60px;
				left: 15%;
			}

			.left-joystick {
				margin-left: 0;
			}

			.right-joystick {
				margin-right: 0;
				margin-left: 10px;
			}

			.other-content {
				margin-top: 20px;
				padding-top: 20px;
			}
		}

		/* Landscape Mode */
		@media only screen and (orientation: landscape) {
			body {
				background-color: #121212;
			}

			.control-section {
				margin-top: 0;
			}

			.joystick-controls {
				display: flex;
				justify-content: space-around;
				width: 70%;
				position: absolute;
				bottom: 60px;
				left: 15%;
			}
		}
	</style>
</head>

<body>
	<h1>Drone</h1>
	<!-- Refresh -->
	<button id="refresh-button" onclick="location.reload();">&#x21bb;</button>
	<!-- Button controls -->
	<div class="control-section">
		<div class="controls">
			<label for="S1">Slider 1</label>
			<input type="range" id="S1" min="0" max="100" value="0" class="slider">
			<input type="range" id="S2" min="0" max="100" value="100" class="slider">
			<label for="S2">Slider 2</label>
			<label for="S3">Slider 3</label>
			<input type="range" id="S3" min="0" max="100" value="0" class="slider">
			<input type="range" id="S4" min="0" max="100" value="100" class="slider">
			<label for="S4">Slider 4</label>
		</div>
		<!-- Black screen placeholder with updated 16:9 aspect ratio -->
		<div class="future-screen"></div>
		<!-- Toggle buttons -->
		<div class="toggle-controls">
			<div class="toggle" id="T1">
				<div class="switch"></div>
			</div>
			<div class="toggle" id="T2">
				<div class="switch"></div>
			</div>
			<div class="toggle" id="T3">
				<div class="switch"></div>
			</div>
			<div class="toggle" id="T4">
				<div class="switch"></div>
			</div>
		</div>
	</div>
	<!-- Joystick controls at the bottom -->
	<div class="joystick-controls">
		<!-- Left joystick -->
		<div class="joystick" id="J1">
			<div class="knob" id="K1"></div>
		</div>
		<!-- Right joystick -->
		<div class="joystick" id="J2">
			<div class="knob" id="K2"></div>
		</div>
	</div>
</body>

<!-- JavaScript for functionality -->
<script>
	// Toggle Switch
	document.querySelectorAll('.toggle').forEach(toggle => {
		toggle.onclick = () => {
			toggle.classList.toggle('active');
		};
	});
	// Joystick Control
	// Helper function for throttling
	function throttle(func, limit) {
		let inThrottle;
		return function (...args) {
			if (!inThrottle) {
				func.apply(this, args);
				inThrottle = true;
				setTimeout(() => (inThrottle = false), limit);
			}
		};
	}
	// Function to calculate distance between two points
	function calculateDistance(x1, y1, x2, y2) {
		return Math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2);
	}
	// Handle joystick movement and throttle
	document.querySelectorAll('.joystick').forEach((joystick) => {
		const knob = joystick.querySelector('.knob');
		let rect;
		let activeTouches = {};  // This will store active touch identifiers and their corresponding joystick
		// Throttle the joystick movement
		// Set interval at 10ms (100Hz)
		const updatePosition = throttle((x, y) => {
			const radius = 35;
			let dx = x - rect.left - 50; // Subtract the center offset of the joystick
			let dy = y - rect.top - 50;  // Subtract the center offset of the joystick
			const distance = Math.sqrt(dx * dx + dy * dy);
			if (distance > radius) {
				dx = (dx / distance) * radius;
				dy = (dy / distance) * radius;
			}
			knob.style.transform = `translate(${dx}px, ${dy}px)`;
		}, 10);
		// Reset the joystick to the center after touch end or mouse up
		const resetJoystick = () => {
			knob.style.transform = `translate(0, 0)`; // Reset the joystick to the center
		};
		// Handle touchstart event: identify closest joystick
		joystick.addEventListener('touchstart', (e) => {
			rect = joystick.getBoundingClientRect();  // Get the joystick bounding rect
			Array.from(e.touches).forEach((touch) => {
				const touchX = touch.clientX;
				const touchY = touch.clientY;
				// Calculate the distance from the touch to the joystick center
				const dist = calculateDistance(
					touchX, touchY, rect.left + rect.width / 2, rect.top + rect.height / 2
				);
				if (dist <= rect.width / 2) {
					activeTouches[touch.identifier] = joystick;  // Associate this touch with the joystick
				}
			});
			// Listen for touchmove and touchend events
			document.addEventListener('touchmove', onTouchMove);
			document.addEventListener('touchend', onTouchEnd);
		});
		// Handle touchmove event: update the joystick's position
		const onTouchMove = (e) => {
			Array.from(e.touches).forEach((touch) => {
				const touchX = touch.clientX;
				const touchY = touch.clientY;
				// If the touch is assigned to a joystick, update its position
				if (activeTouches[touch.identifier] === joystick) {
					updatePosition(touchX, touchY);
				}
			});
		};
		// Handle touchend event: reset the joystick and stop tracking the touch
		const onTouchEnd = (e) => {
			Array.from(e.changedTouches).forEach((touch) => {
				if (activeTouches[touch.identifier] === joystick) {
					delete activeTouches[touch.identifier];  // Stop tracking this touch
					resetJoystick();  // Reset the joystick position
				}
			});
			// If there are no active touches, remove the event listeners
			if (Object.keys(activeTouches).length === 0) {
				document.removeEventListener('touchmove', onTouchMove);
				document.removeEventListener('touchend', onTouchEnd);
			}
		};
		// Mouse events for desktop functionality (same as before)
		joystick.addEventListener('mousedown', (e) => {
			rect = joystick.getBoundingClientRect();
			updatePosition(e.clientX, e.clientY);  // Directly update position when mouse is down
			document.addEventListener('mousemove', onMouseMove);
			document.addEventListener('mouseup', stopDraggingMouse);
		});
		// Update position on mouse move
		const onMouseMove = (e) => {
			updatePosition(e.clientX, e.clientY);
		};
		// Stop mouse dragging and reset position
		const stopDraggingMouse = () => {
			resetJoystick();
			document.removeEventListener('mousemove', onMouseMove);
			document.removeEventListener('mouseup', stopDraggingMouse);
		};
	});

	// Websocket
	// Establish WebSocket connection
	const socket = new WebSocket("ws://" + window.location.hostname + "/CommunicationSocket");
	// Open the WebSocket connection
	socket.onopen = () => {
		console.log("WebSocket is open now.");
	};
	// Handle incoming WebSocket messages (optional)
	socket.onmessage = (event) => {
		console.log("Received data: " + event.data);
	};
	// Handle WebSocket errors
	socket.onerror = (error) => {
		console.error("WebSocket Error: ", error);
	};
	// Handle WebSocket closure
	socket.onclose = (event) => {
		if (event.wasClean) {
			console.log(`Closed cleanly with code ${event.code}`);
		} else {
			console.error(`Connection closed unexpectedly with code ${event.code}`);
		}
	};
	setInterval(() => {
		const controlData = gatherControlData();
		
		if (controlData === null) {
			console.error('Invalid data, not sending');
			return; // Don't send if data is invalid
		}
    	const jsonString = JSON.stringify(controlData);
		// Send the data via WebSocket
		if (socket.readyState === WebSocket.OPEN) {
			socket.send(jsonString);
			console.log('Data sent:', jsonString);
		} else {
			console.error('WebSocket is not open, unable to send data');
		}
	}, 20); // Sends every 20ms (50Hz)

	// Function to gather control data
	function gatherControlData() {
		const data = {
			S1: document.getElementById("S1").value,
			S2: document.getElementById("S2").value,
			S3: document.getElementById("S3").value,
			S4: document.getElementById("S4").value,
			T1: document.getElementById('T1').classList.contains('active'),
			T2: document.getElementById('T2').classList.contains('active'),
			T3: document.getElementById('T3').classList.contains('active'),
			T4: document.getElementById('T4').classList.contains('active'),
			J1: (() => {
				const [X, Y] = (document.querySelector("#J1").querySelector("#K1").style.transform.match(/-?\d+\.\d{2}/g) || [0, 0]).map(parseFloat);
				return { X, Y };
			})(),
			J2: (() => {
				const [X, Y] = (document.querySelector("#J2").querySelector("#K2").style.transform.match(/-?\d+\.\d{2}/g) || [0, 0]).map(parseFloat);
				return { X, Y };
			})(),
		};

		// Validation before sending the data
		// Ensure that no fields contain empty or invalid values
		if (data.S1 && data.S2 && data.S3 && data.S4 &&
			typeof data.T1 === 'boolean' && typeof data.T2 === 'boolean' && 
			typeof data.T3 === 'boolean' && typeof data.T4 === 'boolean' &&
			!isNaN(data.J1.X) && !isNaN(data.J1.Y) &&
			!isNaN(data.J2.X) && !isNaN(data.J2.Y)) {
			return data; // Return valid data only
		} else {
			console.error('Invalid data detected');
			return null; // Return null if invalid
		}
	}
</script>
</body>

</html>
)";