# SMART-METER

## Introduction
Smart electricity meters are electronic devices that allow two-way communication between different stakeholders of the electricity delivery and consumption ecosystem. On a low level, these devices are responsible for calculating electricity consumption information and sending it to the necessary stakeholders. On a higher level, these devices integrate complex communications technology to transmit and receive information. This ability allows the smart meter to satisfy many functional capabilities required by different stakeholders. The Schematic below shows the hardware implementation:

![schematic](https://user-images.githubusercontent.com/44380530/47552375-d3203500-d904-11e8-940a-f74f9882ae56.png)

## Summary 
This paper gives an overview of the design and implementation of a low-cost prepayment smart metering system. The paper will introduce smart metering concepts and then cover the different stakeholder’s requirements in the electricity supply ecosystem. The smart meter implementation uses 2 cores for processing, the inspiration for this comes from multi-core parallel processing which has proven to be more effective than single core processing. The metrology module uses a transformer, a non-invasive current transformer and conditioning circuitry for overvoltage protection. The proposed design uses the Blynk application for remote load control of up-to 3 non-critical loads. The design also integrates data visualisation to communicate consumption information to the utility and the end-user. Results show that the required functionality can be met, however further calibration is required to reduce the error range to a few watts. 


