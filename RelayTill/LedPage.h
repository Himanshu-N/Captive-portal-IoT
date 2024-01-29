const char* led_control_html = R"=====(
<html>
<head>
    <style>
        @import url('https://fonts.googleapis.com/css?family=Montserrat:600&display=swap');
        
        body {
            margin: 0;
            padding: 0;
            display: flex;
            height: 100vh;
            align-items: center;
            justify-content: center;
            background: #fff;
        }

        .button-container {
            display: inline-flex;
            margin: 0 15px;
        }

        .button-container button {
            font-size: 19px;
            letter-spacing: 1px;
            font-family: 'Montserrat', sans-serif;
            border: none;
            color: white;
            padding: 20px 40px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 24px;
            font-weight: bold;
            margin: 10px;
            cursor: pointer;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .button-container .turn-on {
            background-color: #196F3D; /* Green background color */
        }

        .button-container .turn-off {
            background-color: #e74c3c; /* Red background color */
        }

        .button-container button:hover {
            background-color: #333; /* Dark background color on hover */
        }
    </style>
</head>
<body>
    <div class="button-container">
        <button class="turn-on" onclick="turnOn()">Turn On</button>
    </div>
    <div class="button-container">
        <button class="turn-off" onclick="turnOff()">Turn Off</button>
    </div>
    <script>
        function turnOn() {
            fetch('/led/on', { method: 'POST' });
        }

        function turnOff() {
            fetch('/led/off', { method: 'POST' });
        }
    </script>
</body>
</html>
)=====";
