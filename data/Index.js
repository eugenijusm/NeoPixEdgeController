$(function () {
    $(".anim-selector").click(function () {
        DoPost("/api/animation",
        {
            AnimType: $(this).data('value')
        });
    });

    $(".delay-selector").click(function () {
        DoPost("/api/delay",
        {
            ChangeType: $(this).data('value')
        });
    });
});