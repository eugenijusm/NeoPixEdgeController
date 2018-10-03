$(function () {

	$("#BtnSet").click(function () {
		var color1 = hexToRgb($("#PaletteColor1").val());
		var color2 = hexToRgb($("#PaletteColor2").val());
		DoPost("/api/palette",
			{
				clr1: color1,
				clr2: color2
			});
	});

	hexToRgb = function (hex) {
		var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		return result ? {
			r: parseInt(result[1], 16),
			g: parseInt(result[2], 16),
			b: parseInt(result[3], 16)
		} : null;
	}

});