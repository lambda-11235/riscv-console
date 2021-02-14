
#ifndef _REGISTERS_H_
#define _REGISTERS_H_

#define INTERRUPT_ENABLE    (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING   (*((volatile uint32_t *)0x40000004))
#define MTIME_LOW           (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH          (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW        (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH       (*((volatile uint32_t *)0x40000014))
#define CONTROLLER          (*((volatile uint32_t *)0x40000018))


__attribute__((always_inline)) static inline
uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) static inline
void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) static inline
void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) static inline
void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) static inline
void csr_disable_interrupts(void){
    asm volatile ("csrci mstatus, 0x8");
}

__attribute__((always_inline)) static inline
void enable_interrupts(void){
    csr_enable_interrupts();
    INTERRUPT_ENABLE = 1|2|4;
}

__attribute__((always_inline)) static inline
void disable_interrupts(void){
    csr_disable_interrupts();
    INTERRUPT_ENABLE = 0;
}


#endif /* _REGISTERS_H_ */
