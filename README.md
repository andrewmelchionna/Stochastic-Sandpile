# Stochastic-Sandpile
Particles interact and randomly disperse on a graph. We study the time it takes until a 'stable' particle configuration is reached.

# stochasticsandpile 
Simulates the model. Empirically, it seems that the system takes O(n^3) time to stabilize, where n is the number of particles. We attempted to prove this by finding a function of the particle configuration which decreases in expectation, and using the expected decrease to predict the total runtime via the Azuma-Hoeffding inequality: https://en.wikipedia.org/wiki/Azuma%27s_inequality.

# ss_weight_history
One idea for the decreasing quantity mentioned above is the expected time it would take a related interacting particle model, called internal diffusion limited aggregation (IDLA), to stabilize when starting from a given configuration. This code took data on these expected stabilization times, and how they behaved for different particle configurations.

# Free Energy
Yet another idea for a naturally decreasing quantity is Helmholtz Free Energy (equal to energy minus entropy). We played around with different definitions of energy (for example, by treating the particles as electrons with a natural Coulomb potential) and entropy (for example as the amount of symmetry in a configuration). We also tried techniques from optimal transport, where energy is related to the cost of transporting particles to their stable states, and entropy is a measure of the number of ways to transport the particles to stability. This code provided experimental data on these ideas from simulation.

# AllBijections
Calculates certain manifestations of Free Energy for a given configuration.
