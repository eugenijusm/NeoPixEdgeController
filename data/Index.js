$(function () {
	$("#BtnChange").click(function () {
        var animType = $("#animType").val();
        $.post("/api/animation",
			{AnimType: animType},
			function (data, status) { alert(status); });
    });
});