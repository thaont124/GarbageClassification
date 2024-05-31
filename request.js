document.addEventListener("DOMContentLoaded", function() {
    // Replace with your actual API endpoints
    var bin1ApiUrl = "http://192.168.120.123:8080/sieuam1";

    fetchBinData(bin1ApiUrl);
});

function fetchBinData(apiUrl) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var response = this.responseText
            console.log(response);

            var parts = response.split(",");

            var part1 = Math.round(100-parseInt(parts[0], 10)*100/18);
            var part2 = Math.round(100-parseInt(parts[1], 10)*100/19);
            console.log(part2)
            document.getElementById("bin1-percentage").innerText = part1+"%";
            document.getElementById("bin2-percentage").innerText = part2+"%";

            console.log(response);
            setTimeout(function() {
                console.log("Đã đợi 5 giây và thực hiện lệnh tiếp theo");
                // Thực hiện các lệnh tiếp theo ở đây
                fetchBinData(apiUrl);
            }, 5000);
            

        }
    };
    xhttp.open("GET", apiUrl, true);
    xhttp.send();
}
