
var neuron_1_position = mgMath.createVector(0,0,0);
var neuron_1_id = 0;


var neuron_2_position = mgMath.createVector(5,5,5);
var neuron_2_id = 1;


network.addNeuron(neuron_1_id,neuron_1_position);
network.addNeuron(neuron_2_id,neuron_2_position);


var deley;
var synapse_type = -1; //inhibitory
var deley  = 5;
network.addSynapse(neuron_1_id,neuron_2_id,deley,synapse_type);

synapse_type = 1; //excitatory



network.addSynapse(neuron_2_id,neuron_1_id,deley,synapse_type);


network.addSpike(0,neuron_1_id);
network.addSpike(2,neuron_1_id);
network.addSpike(4,neuron_1_id);
network.addSpike(6,neuron_1_id);

network.addSpike(6,neuron_2_id);
network.addSpike(8,neuron_2_id);
network.addSpike(10,neuron_2_id);
network.addSpike(12,neuron_2_id);


animator.animationDuration = 20;
animator.animationStep = 0.01;