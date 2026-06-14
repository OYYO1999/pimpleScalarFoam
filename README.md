# pimpleScalarFoam

## Solver Description

`pimpleScalarFoam` is a custom transient incompressible flow solver developed within the OpenFOAM framework. It is fundamentally extended from the standard `pimpleFoam` solver, which employs the PIMPLE algorithm, a merged PISO-SIMPLE procedure, to resolve the pressure-velocity coupling.

This customized solver is designed for Direct Numerical Simulation (DNS) and Large Eddy Simulation (LES) of forced convection in wall-bounded turbulent flows. In addition to solving the standard incompressible Navier-Stokes equations, `pimpleScalarFoam` simultaneously solves a strictly coupled advection-diffusion equation for a passive scalar field, which represents the temperature field `T`.

To accurately capture the fully developed thermal boundary layer without extending the physical computational domain, a customized volumetric heat source term is directly hard-coded into the scalar transport equation. This source term is strictly proportional to the local instantaneous streamwise velocity. Compared with using the runtime `fvOptions` library, this direct matrix-level implementation avoids additional data-fetching overhead and potential runtime instabilities, while providing better control over the scalar equation, improved energy conservation, and optimized performance in parallel computations.

## Governing Equations

The flow and heat transfer are governed by the unsteady incompressible Navier-Stokes equations and the passive scalar transport equation.

### 1. Continuity Equation

The incompressibility constraint is given by

$$
\nabla \cdot \mathbf{U} = 0
$$

### 2. Momentum Equation

The incompressible Navier-Stokes equation is written as

$$
\frac{\partial \mathbf{U}}{\partial t}
+
\nabla \cdot \left( \mathbf{U} \otimes \mathbf{U} \right)
=========================================================

-\nabla p
+
\nu \nabla^2 \mathbf{U}
+
\mathbf{f}
$$

where:

* $\mathbf{U}$ is the velocity vector field.
* $p$ is the kinematic pressure, defined as the pressure divided by the fluid density, namely $p = P / \rho$.
* $\nu$ is the kinematic viscosity of the fluid.
* $\mathbf{f}$ represents external body forces or artificial momentum source terms, such as the constant mean pressure gradient used to drive the channel flow. In the present implementation, this forcing can be applied through `vectorSemiImplicitSource`.

### 3. Passive Scalar Transport Equation

The passive scalar transport equation is written as

$$
\frac{\partial T}{\partial t}
+
\nabla \cdot \left( \mathbf{U} T \right)
========================================

D \nabla^2 T
+
S_T
$$

where:

* $T$ is the passive scalar field representing the fluid temperature.
* $D$ is the molecular thermal diffusivity.
* $S_T$ is the customized volumetric heat source term.

The thermal diffusivity is defined as

$$
D = \frac{\nu}{Pr}
$$

where $Pr$ is the Prandtl number.

## Customized Volumetric Heat Source

For a fully developed channel flow with a constant axial wall heat flux, the volumetric scalar source term is explicitly defined as

$$
S_T = \gamma U_x
$$

where:

* $\gamma = \partial T_w / \partial x$ is the imposed uniform streamwise wall-temperature gradient.
* $U_x$ is the instantaneous local streamwise velocity component.

Therefore, the scalar equation solved by `pimpleScalarFoam` can be expressed as

$$
\frac{\partial T}{\partial t}
+
\nabla \cdot \left( \mathbf{U} T \right)
========================================

D \nabla^2 T
+
\gamma U_x
$$

This formulation is particularly suitable for DNS and LES of thermally fully developed turbulent channel flows under constant wall heat flux conditions. By introducing the source term directly into the scalar matrix, the solver provides strict control over the scalar transport process and avoids the additional overhead associated with runtime source-term handling.
