/*
 In this example, a synfire chain spiking neural network will be created.
 Then, the network will be totally connected. 
 Finally, spikes times will be added. 
 */


// Creating neurons
var layer_width = 5; // 25 neurons by layer

var layers_count = 4;

var layer_length = 12;

var d_z = 15;
var firstId = 0;

var neurons_by_layer = layer_width * layer_width ;

for (var z = 0 ; z < d_z * layers_count ; z+= d_z)
{
	var base = mgMath.createVector(0,0,z);
	var hExtremity = mgMath.createVector(layer_length,0,z);
	var vExtremity = mgMath.createVector(0,layer_length,z);
	
	network.addNeuronsGrid(
			base,
			hExtremity,
			vExtremity,
			firstId,
			layer_width,//neurons by h
			layer_width //neurons by v
	);
	
	firstId += neurons_by_layer;
	
}

for(var layer = 0 ; layer<layers_count -1 ; ++layer)
{
	
	var source_layer_begin_id = layer * neurons_by_layer;
	var destination_layer_begin_id = (layer+1) * neurons_by_layer;
	
	for(var source_id = source_layer_begin_id;source_id<destination_layer_begin_id;++source_id)
		for(var destination_id = destination_layer_begin_id;destination_id<destination_layer_begin_id+neurons_by_layer;++destination_id)
			network.addSynapse(
				source_id,
				destination_id,
				5,//deley
				1//excitatory synapse
			);
}

var data_directory = '/data/path';
network.loadSpikes(data_directory+'/spikes');
network.loadActivityFromDirectory(data_directory);
