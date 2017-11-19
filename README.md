# LOGinstruments
LOGinstruments: VCV Rack plugins by Leonardo Gabrielli <l.gabrielli@univpm.it>

![family portrait](/res/family-portrait.png?raw=true "LOGinstruments modules")

# LessMess
Tidy up your patching mess with LessMess by assigning labels to your patch cords. Just click on the label and type your text. Connect cables to the left (input) sockets and get the signal out from the right socket. It does not affect the signal whatsoever.

# Compà
Comparator module (2x)

# Britix – the British Matrix
This plugin hosts two different matrices. The top matrix, Britix OP is meant for evaluating expressions of the input signals: the pins have different colors to indicate different operations: sum (RED), difference (BLUE) or product (BLACK). Each row goes into one output. One additional output, called Sigma collects all the previous outputs. If we put, e.g. a red, blue and black pin at positions A1, B1 C1, respectively the output will be:

OUT1 = (INA+INA) + (INA-INB) + (INA*INC)

This can be useful to compute mathematical expression, square some signals and so on.

The bottom matrix, Britix-IO is just an input-output matrix so that each pin connects one input with one output. If we put all the pins on row 2 we will have:

OUT2 = INA + INB + INC

Easy peasy.

# Speck
This plugin is meant for visualizing the Discrete Fourier Transform of a signal. The processing is based on KissFFT source code and employs a Hann window over 2k points. There are two independent channels available and the graph can be plotted on a linear frequency scale or a logarithmic frequency scale, with zoom and adaptive grid. All data is in dB.

# Constant
Provide a DC offset for math purposes. Each knob has a different range.
