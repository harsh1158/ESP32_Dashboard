#ifndef DASHBOARD_H
#define DASHBOARD_H

const char MAIN_page[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>ESP32 Dashboard</title>

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

    background:white;

    border-radius:50%;

    display:flex;

    align-items:center;

    justify-content:center;

    color:#0d47a1;

    font-weight:bold;

    font-size:18px;
}

.title
{
    font-size:28px;

    font-weight:bold;
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
</style>

</head>

<body>

<div class="header">

<div class="logo">
LOGO
</div>

<div class="title">
ESP32 Dashboard
</div>

</div>

<div class="container">

<div class="card">

<h2 class="sectionTitle">Device Configuration</h2>
<div class="form-grid">
<div class="form-group">

<label>ID</label>

<select id="deviceID">

<option value="">Select ID</option>

<option>ID001</option>
<option>ID002</option>
<option>ID003</option>
<option>ID004</option>
<option>ID005</option>
<option>ID006</option>
<option>ID007</option>
<option>ID008</option>
<option>ID009</option>
<option>ID010</option>

</select>

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

<label>Cycle</label>

<input
type="text"
id="cycle"
value="0"
readonly>

</div>

<div class="form-group">

<label>Pulse Cycle</label>

<input
type="text"
id="pulseCycle"
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

<script>

//----------------------------------------------------
// Controls
//----------------------------------------------------

const idDropdown = document.getElementById("deviceID");

const expiryYear = document.getElementById("expiryYear");

const useTime = document.getElementById("useTime");

const cycle = document.getElementById("cycle");

const pulseCycle = document.getElementById("pulseCycle");

const totalPulse = document.getElementById("totalPulse");

const presentDate = document.getElementById("presentDate");

const submitButton = document.getElementById("submitBtn");

const readButton = document.getElementById("readBtn");

const statusMessage = document.getElementById("statusMessage");

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
    .then(response => response.text())
    .then(data =>
    {
        statusMessage.innerHTML = data;
        statusMessage.style.color = "green";
    });
});

//----------------------------------------------------
// ID Change - Load Device Data
//----------------------------------------------------

idDropdown.addEventListener("change", function()
{

    let id = idDropdown.value;


    if(id=="")
    {
        return;
    }


    fetch("/read?id="+id)

    .then(response=>response.json())

    .then(data=>
    {

        expiryYear.value = data.expiry;

        useTime.value = data.useTime;

        cycle.value = data.cycle;

        pulseCycle.value = data.pulse;

        totalPulse.value = data.total;


        statusMessage.innerHTML="Device Loaded";

        statusMessage.style.color="green";


    })

    .catch(error=>
    {

        statusMessage.innerHTML="Read Failed";

        statusMessage.style.color="red";

    });


});

//----------------------------------------------------
// Read
//----------------------------------------------------

readButton.addEventListener("click", function()
{
    let id = idDropdown.value;

    if(id=="")
    {
        statusMessage.innerHTML="Please Select ID";
        statusMessage.style.color="red";
        return;
    }

    fetch("/read?id="+id)

    .then(response=>response.json())

    .then(data=>
    {
        expiryYear.value = data.expiry;

        useTime.value = data.useTime;

        cycle.value = data.cycle;

        pulseCycle.value = data.pulse;

        totalPulse.value = data.total;

        statusMessage.innerHTML="Read Successfully";

        statusMessage.style.color="green";
    });
});

</script>
</body>

</html>

  )rawliteral";

#endif