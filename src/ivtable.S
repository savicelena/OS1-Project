.extern _ZN5Riscv20supervisorTrapSystemEv
.extern _ZN5Riscv18supervisorTrapTimeEv
.extern _ZN5Riscv21supervisorTrapConsoleEv
.extern _ZN5Riscv11unknownTrapEv

.align 4
.global _ZN5Riscv7ivtableEv
.type _ZN5Riscv7ivtableEv, @function
_ZN5Riscv7ivtableEv:
    j _ZN5Riscv20supervisorTrapSystemEv
    j _ZN5Riscv18supervisorTrapTimeEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv21supervisorTrapConsoleEv
    j _ZN5Riscv11unknownTrapEv
    j _ZN5Riscv11unknownTrapEv

//prvi ulaz je za softverski prekid iz najprivilegovanijeg rezima - TAJMER
//deveti ulaz je za hardverski prekid - KONZOLA