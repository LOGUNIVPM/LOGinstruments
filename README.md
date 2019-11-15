# LOGinstruments
LOGinstruments v1: VCV Rack plugins by Leonardo Gabrielli <l.gabrielli@univpm.it>

![family portrait](/res/familyPortrait.png?raw=true "LOGinstruments modules")

# LessMess
Tidy up your patching mess with LessMess by assigning labels to your patch cords. Just click on the label and type your text. Connect cables to the left (input) sockets and get the signal out from the right socket. It does not affect the signal whatsoever.

# Velvet
Velvet noise generator. Inspired by the works from Välimäki et al. (See e.g. http://ieeexplore.ieee.org/abstract/document/6490018/).
Basically velvet noise consists of sparse pulses with amplitude +/-1. When the pulse density gets large it sounds close to white gaussian noise.
Two modes are available: a constant amplitude (+/-10V) and a density-dependent mode, which weights the peaks amplitude with the inverse of the density (see figure above, where a sweep of the density parameter is done)

# Crystal
Easy-to-break signal generator with unpredictable outcomes. Up to 32 oscillators employing an alias-free signal generation technique, it has three input CVs, detune control and laser gem.
Explore it and handle with care. When you're happy lock the settings with the Lock switch. If you want to force a change push the Rst button.

# Speck
Historically, the first FFT plugin in the Rack universe, it simply visualizes the Discrete Fourier Transform of a signal. The processing is based on KissFFT source code and employs a Hann window over 2k points. There are two independent channels available and the graph can be plotted on a linear frequency scale or a logarithmic frequency scale, with zoom and adaptable grid. All data is in dB.

