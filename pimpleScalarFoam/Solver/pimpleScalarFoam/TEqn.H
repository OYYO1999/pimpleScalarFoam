{
    fvScalarMatrix TEqn
    (
        fvm::ddt(T_passive)
      + fvm::div(phi, T_passive)
      - fvm::laplacian(D, T_passive)
     ==
        U.component(0) * gamma
    );

    TEqn.relax();
    
    TEqn.solve();
}
