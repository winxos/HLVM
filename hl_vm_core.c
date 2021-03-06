//
// Created by zx on 2019/10/26.
//

#include "hl_vm_core.h"
#include "hl_vm_optable.h"
#include <string.h>

void hl_vm_start(struct HLVM *vm){
    vm->state=HLVM_STATE_RUNNING;
}
HLVM_RET hl_vm_step(struct HLVM *vm) {
    switch (vm->state)
    {
        case HLVM_STATE_IDLE:
            break;
        case HLVM_STATE_RUNNING:
            vm->ticks++;
            HLVMExec exec= _op_tables[GET_OP(vm->_mems[vm->pc])];
            if(exec==NULL)
            {
                vm->state=HLVM_STATE_HALT;
                return HLVM_INSTRUCTION_NOT_EXIST;
            }
            HLVM_RET ret=exec(vm);
            if(ret!=HLVM_OK)
            {
                vm->state=HLVM_STATE_HALT;
                return ret;
            }
            vm->pc++;
            if(vm->pc>=ROM_SIZE)
            {
                vm->state=HLVM_STATE_HALT;
                return HLVM_PC_OUT_RANGE;
            }
            break;
        case HLVM_STATE_SLEEPING:
            vm->_sleeping--;
            if(vm->_sleeping<=0)
            {
                vm->state=HLVM_STATE_RUNNING;
            }
            break;
        case HLVM_STATE_HALT:
            return HLVM_HALT;
    }
    return HLVM_OK;
}

bool hl_vm__core_test() {
    struct HLVM vm={._mems={0x03000022, 0x04000004},.sp=MEM_SIZE - 1};
    hl_vm_start(&vm);
    hl_vm_step(&vm);
    hl_vm_step(&vm);
    hl_vm_step(&vm);
}

void hl_vm_init(struct HLVM *vm) {
    memset(vm,0, sizeof(struct HLVM));
    vm->sp=MEM_SIZE-1;
}
