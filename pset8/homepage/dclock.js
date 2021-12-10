function dclock()
{
    const d = new Date();
    let hour = d.getHours();
    let min = d.getMinutes();
    let sec = d.getSeconds();

    hour = formatTime(hour);
    min = formatTime(min);
    sec = formatTime(sec);

    let clock = document.getElementById("dclock");
    clock.innerHTML = hour + ":" + min + ":" +  sec;
    setTimeout(dclock, 1000);

}

function formatTime(time)
{
    if (time < 10) {time = "0" + time};
    return time;
}