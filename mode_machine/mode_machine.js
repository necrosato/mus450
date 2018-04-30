var scales = {'major': [1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1],
			  'harmonic_minor': [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1],
			  'melodic_minor': [1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1]};

function get_mode_offset(n, scale) {
	//function returns the offset (0 indexed) 
	//for a given mode n (1 - 7) of a scale (list)
	//TODO : Handle when n is greater than 7
	var ncount = 0;
	for (var i = 0; i < 12; i++) {
		if (scales[scale][i] == 1) {
			ncount++;
		}
		if (ncount == (((n-1) % 7)+1)) {
			return i;
		}
	}
	return 0;
}

function get_mode(n, root, scale) {
	var mode = [];
	var offset = get_mode_offset(n, scale);
	for (var i = 0; i < 12; i++) {	
		var index = (i + offset) % 12;
		if (scales[scale][index] == 1) {
			mode.push(root+(index<i?12:0)+index);
		}
	}
	outlet(0, mode);
	return mode;
}

function get_major_chord(n, depth, mode, root) {
	var chord_num = ((n + mode - 1) % 7); // modal number corresponding to this chord
	var scale = get_major_scale(chord_num, root);
	var chord = [];
	for (var i = 0; i < depth; i++) {
		var index = 0;
	}
}
