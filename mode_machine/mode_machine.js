var scales = {'major': 			[1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1],
			  'harmonic_minor': [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1],
			  'melodic_minor': 	[1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1]};

function get_mode_offset(n, scale) {
	//function returns the offset into scale (0 indexed) for a given mode n of a scale
	var ncount = 0;
    var target = ((n-1) % 7)+1;
    var mult = Math.floor((n-1)/7);
	for (var i = 0; i < 12; i++) {
		if (scales[scale][i] == 1) {
			ncount++;
		}
		if (ncount == target) {
			return (i + (12*mult));
		}
	}
	return 0;
}

function get_mode(n, root, scale) {
    // n: mode number (1 indexed)
    // root: root note of mode
    // scale: key in scales
	var mode = [];
	var offset = get_mode_offset(n, scale);
    var mult = Math.floor(offset/12);
    var new_root = root - (offset % 12); // use this for getting mode AT root
    //var new_root = root; // use this for getting mode FROM root
	for (var i = 0; i < 12; i++) {	
		var index = (i + offset) % 12;
		if (scales[scale][index] == 1) {
			mode.push(new_root+(index<i?12:0)+(mult*12)+index);
		}
	}
	outlet(0, mode);
	return mode;
}

function get_chord(n, depth, mode, root, scale) {
    // n: chord number in relation to start of mode (1 indexed)
    // depth: depth of chord (1 indexed)
    // mode: mode number of scale (1 indexed)
    // root: root note of mode
    // scale: key in scales
	var root_mode = get_mode(mode, root, scale);
	var mode_offset = root_mode[(n-1)%7] - root_mode[0];
	var chord_scale = get_mode(((mode+n-2)%7)+1, root+mode_offset, scale);
	var chord_mult = Math.floor((n - 1) / 7);
    var chord = [];
	for (var i = 0; i < depth; i++) {
		var index = (i * 2) % 7;
        var note_mult = Math.floor((i * 2) / 7);
        chord.push(chord_scale[index]+(note_mult*12)+(chord_mult*12)); 
	}
	outlet(0, chord);
    return chord;
}