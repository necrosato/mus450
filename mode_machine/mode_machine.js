var major_scale = [1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1];
var harmonic_minor_scale = [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1];
var melodic_minor_scale = [1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1];

function get_mode_offset(n, scale) {
	//function returns the offset (0 indexed) 
	//for a given mode n (1 - 7) of a scale (list)
	var offset = 0;
	var ncount = 0;
	for (var i = 0; i < 12; i++) {
		if (scale[i] == 1) {
			ncount++;
		}
		if (ncount == n) {
			offset = i;
			break;
		}
	}
	return offset;
}

function get_major_mode(n, root) {
	var scale = [];
	var offset = get_mode_offset(n, major_scale);
	for (var i = 0; i < 12; i++) {	
		var index = (i + offset) % 12;
		if (major_scale[index] == 1) {
			scale.push(root+(index));
		}
	}
	outlet(0, scale);
	return scale;
}

function get_harmonic_minor_mode(n, root) {
	var scale = [];
	var offset = get_mode_offset(n, harmonic_minor_scale);
	for (var i = 0; i < 12; i++) {	
		var index = (i + offset) % 12;
		if (harmonic_minor_scale[index] == 1) {
			scale.push(root+(index));
		}
	}
	outlet(0, scale);
	return scale;
}

function get_melodic_minor_mode(n, root) {
	var scale = [];
	var offset = get_mode_offset(n, melodic_minor_scale);
	for (var i = 0; i < 12; i++) {	
		var index = (i + offset) % 12;
		if (melodic_minor_scale[index] == 1) {
			scale.push(root+(index));
		}
	}
	outlet(0, scale);
	return scale;
}

function get_major_chord(n, depth, mode, root) {
	var chord_num = ((n + mode - 1) % 7); // modal number corresponding to this chord
	var scale = get_major_scale(chord_num, root);
	var chord = [];
	for (int i = 0; i < depth; i++) {
		var index = 0;
	}
}
