# Stochastic-Sandpile
Indistinguishable particles interact and disperse on a graph. We study the time it takes until a 'stable' particle configuration is reached.

# stochasticsandpile 
Simulates the model. Empirically, it seems that the system takes O(n^3) time to stabilize, where n is the number of particles. We attempted to prove this by finding a function of the particle configuration which decreases in expectation, and using the expected decrease to predict the total runtime via the Azuma-Hoeffding inequality: https://en.wikipedia.org/wiki/Azuma%27s_inequality.

# ss_weight_history
One idea for the decreasing quantity mentioned above is the expected time it would take a related interacting particle model, called internal diffusion limited aggregation (IDLA), to stabilize when starting from a given configuration. This code took data on these expected stabilization times, and how they behaved for different particle configurations.
