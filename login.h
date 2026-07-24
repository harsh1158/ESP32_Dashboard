#ifndef LOGIN_H
#define LOGIN_H

const char LOGIN_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>

<meta charset="UTF-8">
<meta name="viewport"
content="width=device-width, initial-scale=1.0">
<title>ESP32 Login</title>
<style>

*
{
    margin:0;
    padding:0;
    box-sizing:border-box;
    font-family:Arial;
}

body
{
    background:#eef2f7;
    display:flex;
    justify-content:center;
    align-items:center;
    height:100vh;
}

.card
{
    width:480px;
    background:white;
    border-radius:12px;
    padding:40px;
    box-shadow:0px 5px 20px rgba(0,0,0,0.2);
}

h2
{
    text-align:center;
    margin-bottom:30px;
    color:#1565C0;
    font-size:34px;
}

input
{
    width:100%;
    padding:16px;
    margin-top:12px;
    margin-bottom:24px;
    font-size:18px;
    border-radius:8px;
}

button
{
    width:100%;
    height:55px;
    background:#1565C0;
    color:white;
    font-size:22px;
    font-weight:bold;
    border:none;
    border-radius:8px;
    cursor:pointer;
}

button:hover
{
    background:#0D47A1;
}

.passwordBox button:hover
{
    background:transparent;
}

#status
{
    margin-top:20px;
    text-align:center;
    color:red;
    font-weight:bold;
    font-size:18px;
}

.passwordBox
{
    position:relative;
    width:100%;
}

.passwordBox input
{
    width:100%;
    padding:12px 50px 12px 12px;
}

.passwordBox button
{
    position:absolute;
    right:10px;
    top:50%;
    transform:translateY(-50%);

    width:35px;
    height:35px;

    background:transparent;
    border:none;
    cursor:pointer;

    font-size:22px;
    color:#555;

    padding:0;
    margin:0;
}
</style>
</head>
<body>
<div class="card">
<h2>Login</h2>
<input
type="text"
id="username"
placeholder="Username">

<div class="passwordBox">
<input
type="password"
id="password"
placeholder="Password">
<button
type="button"
id="togglePassword">
👁
</button>

</div>

<button id="loginButton">
Login
</button>

<div id="status"></div>
</div>

<script>

const loginButton =
document.getElementById("loginButton");

const status =
document.getElementById("status");

const togglePassword =
document.getElementById("togglePassword");

const passwordInput =
document.getElementById("password");

loginButton.addEventListener("click", function()
{
    let username =
    document.getElementById("username").value;

    let password =
    document.getElementById("password").value;

    const formData =
    new URLSearchParams();

    formData.append("username", username);
    formData.append("password", password);

    fetch("/login",
    {
        method:"POST",
        headers:
        {
            "Content-Type":
            "application/x-www-form-urlencoded"
        },
        body:formData.toString()
    })
    .then(response => response.text())
    .then(data =>
{
    if(data == "Login Success")
    {
        window.location.href = "/";
    }
    else
    {
        status.innerHTML = data;
        status.style.color = "red";
    }
})
    .catch(error =>
    {
        status.innerHTML =
        "Login Failed";
        status.style.color =
        "red";
    });
});

togglePassword.addEventListener("click", function()
{
    if(passwordInput.type == "password")
    {
        passwordInput.type = "text";
    }
    else
    {
        passwordInput.type = "password";
    }
});
</script>
</body>
</html>

)rawliteral";

#endif