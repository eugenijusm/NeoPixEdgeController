$(function () {
    DoPost = function (url, data) {
        var now = new Date(Date.now());
        var currentTime = now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds();
        $.ajax({
            type: "POST",
            url: "/api/animation",
            data: data
        })
            .done(function () {
                $("#ApiResult").prepend(currentTime + " Ok<br/>");                
            })
            .fail(function () {
                $("#ApiResult").prepend(currentTime + " FAIL!<br/>");
            });

    }
});