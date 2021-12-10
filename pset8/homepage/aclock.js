function aclock()
{
    const hourHand = document.querySelector(".hour");
    const minHand = document.querySelector(".min");
    const secHand = document.querySelector(".sec");

    setInterval(function() {
        const d = new Date();

        const secDeg = d.getSeconds() / 60 * 360 - 90;
        const minDeg = d.getMinutes() / 60 * 360 - 90;
        const hourDeg = (d.getHours() / 12 * 360) + d.getMinutes() / 60 * (1 / 12 * 360) - 90;

        hourHand.style.transform = `rotate(${hourDeg}deg)`;
        minHand.style.transform = `rotate(${minDeg}deg)`;
        secHand.style.transform = `rotate(${secDeg}deg)`;
    }, 1000);

}