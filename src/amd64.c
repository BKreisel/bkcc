#include <stdbool.h>
#include <stdlib.h>
#include "amd64.h"
#include "error.h"
#include "parser.h"

// Prototypes -------------------------------------------------------------------------------------
bool amd64_compile(PASTNode root, const char* fname);
AMD64_GEN_REG amd64_ast(PAMD64_CTX pCtx, PASTNode root);
const char* amd64_reg_name(AMD64_GEN_REG reg);
void amd64_printint(PAMD64_CTX pCtx);
void amd64_push(PAMD64_CTX pCtx, AMD64_GEN_REG reg);
void amd64_pop(PAMD64_CTX pCtx, AMD64_GEN_REG reg);
void amd64_mov(PAMD64_CTX pCtx, char* src, char* dst);
void amd64_call(PAMD64_CTX pCtx, char* func);
void amd64_enter(PAMD64_CTX pCtx);
void amd64_leave(PAMD64_CTX pCtx);
AMD64_GEN_REG alloc_register(PAMD64_CTX pCtx);
void free_register(PAMD64_CTX pCtx, AMD64_GEN_REG reg);
AMD64_GEN_REG amd64_load(PAMD64_CTX pCtx, int val);
AMD64_GEN_REG amd64_add(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right);
AMD64_GEN_REG amd64_sub(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right);
AMD64_GEN_REG amd64_multiply(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right);
AMD64_GEN_REG amd64_divide(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right);

// ------------------------------------------------------------------------------------------------
bool amd64_compile(PASTNode root, const char* fname)
{
    AMD64_CTX ctx = {0};
    ctx.fd = fopen(fname, "w");
    if(NULL == ctx.fd) {
        set_error(ERR_FOPEN, "output file: %s", fname);
    }

    fputs(
        "; --------------------------------------------------------------------------------------------------\n"
        "; bkcc generated ASM\n"
        "; --------------------------------------------------------------------------------------------------\n"
        "\n"
        "GLOBAL main\n"
        "EXTERN printf\n"
        "\n"
        "SECTION .data\n"
        "\tprintint_fmt:\tdb\t\"%d\", 0xA, 0\t; Format String for printint\n"
        "\nSECTION .text\n",
        ctx.fd
    );
    amd64_printint(&ctx);
    fputs("main:\n", ctx.fd);
    amd64_enter(&ctx);
    
    clear_error();
    AMD64_GEN_REG result = amd64_ast(&ctx, root);
    if(ERR_OK != get_error())
    {
        return false;
    }
    amd64_mov(&ctx, "rdi", (char*)amd64_reg_name(result));
    amd64_call(&ctx, "printint");
    amd64_leave(&ctx);
    fflush(ctx.fd);

    return true;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_ast(PAMD64_CTX pCtx, PASTNode root)
{
    AMD64_GEN_REG left, right;

    if(NULL != root->lchild) {
        left = amd64_ast(pCtx, root->lchild);
    }

    if(NULL != root->rchild) {
        right = amd64_ast(pCtx, root->rchild);
    }

    switch (root->op) {
        case AST_MULTIPLY:
            return amd64_multiply(pCtx, left, right);
        case AST_DIVIDE:
            return amd64_divide(pCtx, left, right);
        case AST_ADD:
            return amd64_add(pCtx, left, right);
        case AST_SUB:
            return amd64_sub(pCtx, left, right);
        case AST_INTEGER:
            return amd64_load(pCtx, root->value);
        
        default:
            set_error(ERR_AST_OP, "Bad AST Operator: %s (%d)", 
                      ast_name(root->op), root->value);
            return 0;
    }
}

// ------------------------------------------------------------------------------------------------
const char* amd64_reg_name(AMD64_GEN_REG reg)
{
    switch(reg)
    {
        case RAX:
            return "rax";
        case RBX:
            return "rbx";
        case RCX:
            return "rcx";
        case RDX:
            return "rdx";
        case RBP:
            return "rbp";
        case R8:
            return "r8";
        case R9:
            return "r9";
        case R10:
            return "r10";
        case R11:
            return "r11";
        case R12:
            return "r12";
        case R13:
            return "r13";
        case R14:
            return "r14";
        case R15:
            return "r15";

        return "Unknown";
    }
}

// ------------------------------------------------------------------------------------------------
void amd64_printint(PAMD64_CTX pCtx)
{
    fputs("printint:\n", pCtx->fd);
    amd64_enter(pCtx);
    amd64_mov(pCtx, "rax", "rdi");
    amd64_mov(pCtx, "rdi", "printint_fmt");
    amd64_mov(pCtx, "rsi", "rax");
    amd64_call(pCtx, "printf");
    amd64_leave(pCtx);        
}

// ------------------------------------------------------------------------------------------------
void amd64_push(PAMD64_CTX pCtx, AMD64_GEN_REG reg)
{
    fprintf(pCtx->fd, "\tpush %s\n", amd64_reg_name(reg));
}

// ------------------------------------------------------------------------------------------------
void amd64_pop(PAMD64_CTX pCtx, AMD64_GEN_REG reg)
{
    fprintf(pCtx->fd, "\tpop %s\n", amd64_reg_name(reg));
}

// ------------------------------------------------------------------------------------------------
void amd64_mov(PAMD64_CTX pCtx, char* src, char* dst)
{
    fprintf(pCtx->fd, "\tmov %s, %s\n", src, dst);
}

// ------------------------------------------------------------------------------------------------
void amd64_call(PAMD64_CTX pCtx, char* func)
{
    fprintf(pCtx->fd, "\tcall %s\n", func);
}

// ------------------------------------------------------------------------------------------------
void amd64_enter(PAMD64_CTX pCtx)
{
    fputs("\tpush rbp\n\tmov rbp, rsp\n", pCtx->fd);
}

// ------------------------------------------------------------------------------------------------
void amd64_leave(PAMD64_CTX pCtx)
{
    fputs("\tmov rsp, rbp\n\tpop rbp\n\tret\n", pCtx->fd);
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG alloc_register(PAMD64_CTX pCtx)
{
    for(int x=0; x < AMD64_GEN_REG_COUNT; x++) {
        if(!pCtx->reg_state[x]) {
            pCtx->reg_state[x] = true;
            return (AMD64_GEN_REG)x;
        }
    }
    set_error(ERR_COMPILER, "Ran out of AMD64 Registers");
    print_error();
    exit(1);
}

// ------------------------------------------------------------------------------------------------
void free_register(PAMD64_CTX pCtx, AMD64_GEN_REG reg)
{
    pCtx->reg_state[(int)reg] = false;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_load(PAMD64_CTX pCtx, int val)
{
    AMD64_GEN_REG dest = alloc_register(pCtx);
    fprintf(pCtx->fd, "\tmov %s, %d\n", (char*)amd64_reg_name(dest), val);
    return dest;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_add(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right)
{
    fprintf(pCtx->fd, "\tadd %s, %s\n", amd64_reg_name(right), amd64_reg_name(left));
    free_register(pCtx, left);
    return right;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_sub(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right)
{
    fprintf(pCtx->fd, "\tsub %s, %s\n", amd64_reg_name(left), amd64_reg_name(right));
    free_register(pCtx, right);
    return left;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_multiply(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right)
{
    fprintf(pCtx->fd, "\timul %s, %s\n", amd64_reg_name(left), amd64_reg_name(right));
    free_register(pCtx, right);
    return left;
}

// ------------------------------------------------------------------------------------------------
AMD64_GEN_REG amd64_divide(PAMD64_CTX pCtx, AMD64_GEN_REG left, AMD64_GEN_REG right)
{
    if(pCtx->reg_state[RDX]) {
        amd64_push(pCtx, RDX);
    }
    if(pCtx->reg_state[RAX]) {
        amd64_push(pCtx, RAX);
    }

    amd64_mov(pCtx, "rdx", "0");
    amd64_mov(pCtx, "rax", (char*)amd64_reg_name(left));
    fprintf(pCtx->fd, "\tidiv %s\n", amd64_reg_name(right));
    amd64_mov(pCtx, (char*)amd64_reg_name(left), "rax");

    if(left != RAX && pCtx->reg_state[RAX]) {
        amd64_pop(pCtx, RAX);
    } else if (pCtx->reg_state[RAX]) {
        AMD64_GEN_REG tmp = alloc_register(pCtx);
        amd64_pop(pCtx, tmp);
        free_register(pCtx, tmp);
    }
    if(pCtx->reg_state[RDX]) {
        amd64_pop(pCtx, RDX);
    }
    free_register(pCtx, right);
    return left;
}