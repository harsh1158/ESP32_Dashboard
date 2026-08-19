#ifndef DASHBOARD_H
#define DASHBOARD_H

const char MAIN_page[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title> Dashboard </title>
<style>

*
{
    margin:0;
    padding:0;
    box-sizing:border-box;
    font-family:Arial, Helvetica, sans-serif;
}

body
{
    background:#eef2f7;
}

.header
{
    width:100%;
    height:75px;
    background:#0d47a1;
    color:white;
    display:flex;
    align-items:center;
    justify-content:space-between;
    padding-left:25px;
    padding-right:25px;
    box-shadow:0px 3px 10px rgba(0,0,0,0.2);
}

.logo
{
    width:55px;
    height:55px;
    display:flex;
    align-items:center;
    justify-content:center;
    background:white;
    color:#0d47a1;
    border-radius:50%;
    font-size:18px;
    font-weight:bold;
    flex-shrink:0;
}

.title
{
    flex:1;
    text-align:center;
    font-size:30px;
    font-weight:bold;
    color:white;
}

.container
{
    width:95%;
    max-width:900px;
    margin:35px auto;
}

.card
{
    background:white;
    border-radius:12px;
    padding:30px;
    box-shadow:0px 4px 15px rgba(0,0,0,0.15);
}
.sectionTitle
{
    color:#0d47a1;
    margin-bottom:25px;
    text-align:center;
}

.form-grid
{
    display:block;
}

.form-group
{
    display:flex;
    flex-direction:column;
    margin-bottom:20px;
}

label
{
    margin-bottom:8px;
    font-weight:bold;
    color:#333;
}

input,
select
{
    width:100%;
    padding:14px;
    font-size:16px;
    border:2px solid #d8d8d8;
    border-radius:10px;
    outline:none;
    transition:0.3s;
}

input:focus,
select:focus
{
    border-color:#1565C0;
    box-shadow:0 0 10px rgba(21,101,192,0.3);
}

input[readonly]
{
    background:#eeeeee;
}

.buttonRow
{
    display:flex;
    width:100%;
    gap:20px;
    margin-top:35px;
}

.buttonRow button
{
    flex:1;
    height:55px;
    width:100%;
    font-size:22px;
    font-weight:bold;
    border:none;
    border-radius:10px;
    background:#1565C0;
    color:white;
    cursor:pointer;
    transition:0.3s;
}

.buttonRow button:hover
{
    background:#0D47A1;
    transform:translateY(-3px);
    box-shadow:0px 8px 18px rgba(0,0,0,0.25);
}
.statusBox
{
    margin-top:30px;
    padding:18px;
    border-left:6px solid #4CAF50;
    background:#E8F5E9;
    color:#2E7D32;
    font-weight:bold;
    border-radius:8px;
}
@media (max-width:700px)
{
    .buttonRow
    {
        flex-direction:column;
    }
}

.menuContainer
{
    position:relative;
    flex-shrink:0;
}

.menuButton
{
    padding:12px 24px;
    background:#1565C0;
    color:white;
    border:none;
    border-radius:8px;
    cursor:pointer;
    font-size:18px;
    font-weight:bold;
}

.dropdownMenu
{
    display:none;
    position:absolute;
    right:0;
    top:58px;
    background:white;
    min-width:240px;
    border-radius:6px;
    box-shadow:0 4px 12px rgba(0,0,0,0.2);
    overflow:hidden;
    z-index:1000;
}

.dropdownMenu button
{
    width:100%;
    padding:16px 20px;
    background:white;
    color:black;
    border:none;
    text-align:left;
    cursor:pointer;
    font-size:18px;
    font-weight:bold;
}

.dropdownMenu button:hover
{
    background:#eeeeee;
}

.modal
{
    display:none;
    position:fixed;
    left:0;
    top:0;
    width:100%;
    height:100%;
    background:rgba(0,0,0,0.4);
    z-index:9999;
}

.modalContent
{
    width:430px;
    margin:120px auto;
    background:white;
    border-radius:10px;
    padding:25px;
}

.modalContent h2
{
    text-align:center;
    color:#1565C0;
    margin-bottom:20px;
}

.modalContent label
{
    display:block;
    margin-top:12px;
    margin-bottom:5px;
}

.modalContent input
{
    width:100%;
    padding:10px;
    margin-bottom:10px;
}
</style>

</head>
<body>
<div class="header">

<div class="logo">
LOGO
</div>

<div class="title">
Dashboard
</div>

<div class="menuContainer">

<button
class="menuButton"
id="accountButton">
Account ▼
</button>

<div
class="dropdownMenu"
id="accountMenu">

<button id="changePasswordMenu">
Change Password
</button>

<button id="logoutMenu">
Logout
</button>
</div>
</div>
</div>

<div class="container">
<div class="card">
<h2 class="sectionTitle">Device Configuration</h2>
<div class="form-grid">
<div class="form-group">

<label>Probe Name</label>
<select id="deviceID">

<option value="">Select Probe</option>

<option value="1">SP_275X12MM_80</option>
<option value="2">SP_275X12MM_120</option>
<option value="3">SP_375X12MM_80</option>
<option value="4">SP_300X12MM_80</option>
<option value="5">SP_325X12MM_80</option>
<option value="6">SP_350X12MM_80</option>
<option value="7">MP_450X60MM_150</option>
<option value="8">MP_500X60MM_150</option>
<option value="9">MP_550X60MM_150</option>
<option value="10">SP_250X12MM_80</option>
<option value="11">SP_250X12MM_120</option>
<option value="12">SP_325X12MM_120</option>
<option value="13">SP_400x12MM_80</option>
<option value="14">MP_600X60MM_150</option>
<option value="15">MP_700X60MM_150</option>
<option value="16">SP_350X12MM_120</option>
<option value="17">MP_350X60MM_300</option>
<option value="18">MP_350X40MM_150</option>
<option value="19">MP_700X60MM_300</option>
<option value="20">MP_350X60MM_150</option>
<option value="21">MP_650X60MM_150</option>
<option value="22">MP_400X60MM_150</option>
<option value="23">MP_400X40MM_150</option>
<option value="24">SP_300X12MM_120</option>
<option value="25">MP_500X60MM_300</option>
<option value="26">MP_300X40MM_150</option>
<option value="27">MP_400X60MM_300</option>
<option value="28">MP_450X60MM_300</option>
<option value="29">MP_550X60MM_300</option>
<option value="30">MP_600X60MM_300</option>
<option value="31">MP_650X60MM_300</option>
<option value="32">SP_375X12MM_120</option>
<option value="33">MP_250X40MM_150</option>
<option value="34">SP_400x12MM_120</option>
<option value="35">MP_800x30MM_300</option>
<option value="36">MP_900x30MM_300</option>
<option value="37">MP_1000x30MM_300</option>
<option value="38">MP_1200x30MM_300</option>
<option value="39">MP_750X60MM_150</option>
<option value="40">MP_800X60MM_150</option>
<option value="41">MP_250X80MM_400</option>
<option value="42">MP_300X80MM_400</option>
<option value="43">MP_350X80MM_400</option>
<option value="44">MP_400X80MM_400</option>
<option value="45">MP_500X80MM_400</option>
<option value="46">MP_600X80MM_400</option>
<option value="47">MP_750X60MM_300</option>
<option value="48">MP_800X60MM_300</option>

</select>
</div>

<div class="form-group">
    <label>Pulse Strategy</label>
    <input
    type="text"
    id="pulseStrategy"
    value="0"
    readonly>
</div>

<div class="form-group">
<label>Total Cycle</label>
<input
type="text"
id="totalCycle"
value="0"
readonly>
</div>

<div class="form-group">
<label>Total Pulse</label>
<input
type="text"
id="totalPulse"
value="0"
readonly>
</div>

<div class="form-group">
<label>Present Date</label>
<input
type="text"
id="presentDate"
value="--/--/----"
readonly>

</div>

<div class="form-group">
<label>Expiry Year</label>
<select id="expiryYear">

<option value="1">1 Year</option>
<option value="2">2 Years</option>
<option value="3">3 Years</option>
<option value="4">4 Years</option>
<option value="5">5 Years</option>
<option value="6">6 Years</option>
<option value="7">7 Years</option>
<option value="8">8 Years</option>
<option value="9">9 Years</option>
<option value="10">10 Years</option>

</select>
</div>

<div class="form-group">
<label>My Use Time</label>
<select id="useTime">
</select>
</div>

<div class="buttonRow">
<button id="submitBtn">
SUBMIT
</button>

<button id="readBtn">
READ
</button>
</div>

<div class="statusBox">
<strong>Status :</strong>
<span id="statusMessage">
Ready
</span>
</div>

</div>
</div>

<div id="changePasswordModal" class="modal">

<div class="modalContent">

<h2>Change Login Credentials</h2>

<label>Current Password</label>

<input
type="password"
id="currentPassword">

<label>New Username</label>

<input
type="text"
id="newUsername">

<label>New Password</label>

<input
type="password"
id="newPassword">
<label>Confirm Password</label>
<input
type="password"
id="confirmPassword">
<div class="buttonRow">

<button id="saveLoginButton">
Save
</button>

<button id="cancelLoginButton">
Cancel
</button>
</div>

<div id="loginStatus">
Ready
</div>
</div>
</div>

<script>
let lastProbeConnectionState = null;
setInterval(checkProbeStatus, 1000);

checkProbeStatus();
//----------------------------------------------------
// Controls
//----------------------------------------------------

const idDropdown = document.getElementById("deviceID");
const expiryYear = document.getElementById("expiryYear");
const useTime = document.getElementById("useTime");
const pulseStrategy = document.getElementById("pulseStrategy");
const totalCycle = document.getElementById("totalCycle");
const totalPulse = document.getElementById("totalPulse");
const presentDate = document.getElementById("presentDate");
const submitButton = document.getElementById("submitBtn");
const readButton = document.getElementById("readBtn");
const logoutButton = document.getElementById("logoutBtn");
const statusMessage = document.getElementById("statusMessage");

const currentPassword =
document.getElementById("currentPassword");

const newUsername =
document.getElementById("newUsername");

const newPassword =
document.getElementById("newPassword");

const confirmPassword =
document.getElementById("confirmPassword");

const loginStatus =
document.getElementById("loginStatus");

const accountButton =
document.getElementById("accountButton");

const accountMenu =
document.getElementById("accountMenu");

const changePasswordMenu =
document.getElementById("changePasswordMenu");

const logoutMenu =
document.getElementById("logoutMenu");

const changePasswordModal =
document.getElementById("changePasswordModal");

const saveLoginButton =
document.getElementById("saveLoginButton");

const cancelLoginButton =
document.getElementById("cancelLoginButton");

//----------------------------------------------------
// Probe Connection Status
//----------------------------------------------------

function checkProbeStatus()
{
    fetch("/probeStatus")
    .then(response =>
    {
        if(!response.ok)
        {
            throw new Error("Probe status failed");
        }

        return response.json();
    })
    .then(data =>
    {
        // ---------------------------------------------
        // Probe CONNECTED
        // ---------------------------------------------
        if(data.connected)
        {
            // Only update Status when connection
            // actually changes from disconnected -> connected
            if(lastProbeConnectionState !== true)
            {
                statusMessage.innerHTML = "Probe Connected";
                statusMessage.style.color = "green";
            }
            lastProbeConnectionState = true;

            // Enable dashboard controls
            idDropdown.disabled = false;
            expiryYear.disabled = false;
            useTime.disabled = false;
            submitButton.disabled = false;
            readButton.disabled = false;
        } else
        {
            // Only update Status when connection
            // actually changes from connected -> disconnected
            if(lastProbeConnectionState !== false)
            {
                statusMessage.innerHTML = "Probe Disconnected";
                statusMessage.style.color = "red";
            }
            lastProbeConnectionState = false;

            // Disable dashboard controls
            idDropdown.disabled = true;
            expiryYear.disabled = true;
            useTime.disabled = true;
            submitButton.disabled = true;
            readButton.disabled = true;
        }
    })
    .catch(error =>
    {
        console.log(error);
        if(lastProbeConnectionState !== false)
        {
            statusMessage.innerHTML = "Probe Disconnected";
            statusMessage.style.color = "red";
        }
        lastProbeConnectionState = false;

        idDropdown.disabled = true;
        expiryYear.disabled = true;
        useTime.disabled = true;
        submitButton.disabled = true;
        readButton.disabled = true;
    });
}

//----------------------------------------------------
// Status Message
//----------------------------------------------------

function setStatusMessage(message, color)
{
    statusMessage.innerHTML = message;
    statusMessage.style.color = color;
}

//----------------------------------------------------
// Today's Date
//----------------------------------------------------

let today = new Date();
let day = String(today.getDate()).padStart(2,'0');
let month = String(today.getMonth()+1).padStart(2,'0');
let year = today.getFullYear();
presentDate.value = day + "/" + month + "/" + year;

//----------------------------------------------------
// Use Time
//----------------------------------------------------

for(let i=1;i<=100;i++)
{
    let option=document.createElement("option");
    option.value=i;
    option.text=i+" Hour";
    if(i==6)
    {
        option.selected=true;
    }
    useTime.appendChild(option);
}

//----------------------------------------------------
// Submit
//----------------------------------------------------

submitButton.addEventListener("click", function()
{
    let id = idDropdown.value;
    let expiry = expiryYear.value;
    let use = useTime.value;
    if(id=="")
    {
        statusMessage.innerHTML = "Please Select ID";
        statusMessage.style.color = "red";
        return;
    }
    const formData = new URLSearchParams();

    formData.append("id", id);
    formData.append("expiry", expiry);
    formData.append("useTime", use);

    fetch("/save",
    {
        method: "POST",
        headers:
        {
            "Content-Type": "application/x-www-form-urlencoded"
        },
        body: formData.toString()
    })
    .then(response =>
{
    if(!response.ok)
    {
        throw new Error("Save failed");
    }

    return response.json();
})
.then(data =>
{
    pulseStrategy.value = data.pulseStrategy;
    totalCycle.value = data.totalCycle;
    totalPulse.value = data.totalPulse;

    expiryYear.value = data.expiry;
    useTime.value = data.useTime;

    setStatusMessage(
    data.probeName + " Saved Successfully",
    "green"
    );
})
.catch(error =>
{
    console.log(error);

    statusMessage.innerHTML =
        "Save Failed";

    statusMessage.style.color = "red";
});
});

//----------------------------------------------------
// ID Change
//----------------------------------------------------

idDropdown.addEventListener("change", function()
{
    // Only select the probe.
    // EEPROM will NOT be read here.

    let id = idDropdown.value;

    if(id == "")
    {
        statusMessage.innerHTML = "Please Select ID";
        statusMessage.style.color = "red";
        return;
    }

        setStatusMessage(
        "Probe " + id + " selected. Click Submit.",
        "black"
        );
});

//----------------------------------------------------
// Read 
//----------------------------------------------------

readButton.addEventListener("click", async function()
{
    let id = idDropdown.value;

    /*if(id == "")
    {
        statusMessage.innerHTML = "Please Select ID";
        statusMessage.style.color = "red";
        return;
    }*/

    const MAX_RETRIES = 4;
    const RETRY_DELAY = 150;

    statusMessage.innerHTML =
        "Reading Probe " + id + "...";

    statusMessage.style.color = "black";

    // Prevent repeated manual clicks while retrying
    readButton.disabled = true;
    submitButton.disabled = true;

    for(let attempt = 1; attempt <= MAX_RETRIES; attempt++)
    {
        try
        {
            console.log(
                "EEPROM READ attempt " +
                attempt +
                "/" +
                MAX_RETRIES
            );
            const response = await fetch(
                "/read?id=" + encodeURIComponent(id)
            );

            if(!response.ok)
            {
                throw new Error("EEPROM read failed");
            }

            const data = await response.json();

            idDropdown.value = data.id;
            expiryYear.value = data.expiry;
            useTime.value = data.useTime;

            pulseStrategy.value = data.pulseStrategy;
            totalCycle.value = data.totalCycle;
            totalPulse.value = data.totalPulse;

            if (data.expired)
            {
                setStatusMessage(
                    "Probe Expired",
                    "red"
                );
            } else
            {
                setStatusMessage(
                    data.probeName + " Read Successfully",
                    "green"
                );
            }

            console.log(
                "EEPROM READ SUCCESS on attempt " +
                attempt
            );

            // Stop retrying
            readButton.disabled = false;
            submitButton.disabled = false;

            return;
        }
        catch(error)
        {
            console.log(
                "EEPROM READ FAILED - Attempt " +
                attempt
            );

            if(attempt < MAX_RETRIES)
            {
                statusMessage.innerHTML =
                    "Read failed. Retrying (" +
                    (attempt + 1) +
                    "/" +
                    MAX_RETRIES +
                    ")...";

                statusMessage.style.color = "orange";

                await new Promise(resolve =>
                    setTimeout(resolve, RETRY_DELAY)
                );
            }
        }
    }
    statusMessage.innerHTML =
        "Probe Read Failed";

    statusMessage.style.color = "red";

    readButton.disabled = false;
    submitButton.disabled = false;
});

saveLoginButton.addEventListener("click", function()
{
    const formData = new URLSearchParams();

    formData.append("currentPassword",
                    currentPassword.value);

    formData.append("newUsername",
                    newUsername.value);

    formData.append("newPassword",
                    newPassword.value);

    formData.append("confirmPassword",
                    confirmPassword.value);

    fetch("/changeLogin",
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
        loginStatus.innerHTML = data;

        if(data == "Login Updated Successfully")
{
    loginStatus.innerHTML = data;
    loginStatus.style.color = "green";

    currentPassword.value = "";
    newUsername.value = "";
    newPassword.value = "";
    confirmPassword.value = "";

    setTimeout(function()
    {
        fetch("/logout")
        .then(response => response.text())
        .then(data =>
        {
            window.location.href = "/";
        });

    }, 2000);
}
        else
        {
            loginStatus.style.color = "red";
        }
    })

    .catch(error =>
    {
        loginStatus.innerHTML = "Update Failed";
        loginStatus.style.color = "red";
    });

});

accountButton.addEventListener("click", function()
{
    if(accountMenu.style.display == "block")
    {
        accountMenu.style.display = "none";
    }
    else
    {
        accountMenu.style.display = "block";
    }
});

document.addEventListener("click", function(event)
{
    if(!accountButton.contains(event.target) &&
       !accountMenu.contains(event.target))
    {
        accountMenu.style.display = "none";
    }
});

changePasswordMenu.addEventListener("click", function(event)
{
    event.stopPropagation();
    accountMenu.style.display = "none";
    changePasswordModal.style.display = "block";
});

logoutMenu.addEventListener("click", function(event)
{
    event.stopPropagation();

    accountMenu.style.display = "none";

    fetch("/logout")
    .then(response => response.text())
    .then(data =>
    {
        window.location.href = "/";
    });
});

cancelLoginButton.addEventListener("click", function()
{
    changePasswordModal.style.display = "none";
});

window.addEventListener("click", function(event)
{
    if(event.target == changePasswordModal)
    {
        changePasswordModal.style.display = "none";
    }
});
</script>
</body>
</html>

  )rawliteral";

#endif