$(function(){
	
$( "#BtnSet" ).click(function() {
	//alert($("#PaletteColor1").val());
	var color1 = hexToRgb($("#PaletteColor1").val());
	//alert(color1.r);
	//alert(window.location.pathname);
	$.post("/api/palette",
	{clr1:color1},
	//{suggest: 1},
	function(data,status){alert(status);});
});

hexToRgb = function(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

});