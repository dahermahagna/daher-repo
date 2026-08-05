#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/

#define MAX(a, b) (a) > (b) ? (a) : (b)

static int s_count = 0;

static void print_count()
{
    printf("s_count: %d\n",s_count);
}


/*--------------------------- public transport ---------------------------*/

typedef struct public_trasport PublicTransport;

typedef void (*PT_vtfunc_t)(PublicTransport *const this_);

void PTCtor(PublicTransport * const this_);
void PTDtor(PublicTransport * const this_);
void PTCctor(PublicTransport *const this_, PublicTransport *const other_);
int PTGetID(PublicTransport * const this_);
void PTDisplay(PublicTransport * const this_);

typedef struct PT_virtual_table
{
    PT_vtfunc_t dtor;
    PT_vtfunc_t display;

}PT_vtable_t;

struct public_trasport
{
    const PT_vtable_t *vptr;
    int m_license_plate;

};

static const PT_vtable_t pt_vtable = {PTDtor, PTDisplay};

void PTCtor(PublicTransport *const this_)
{
    this_->vptr = &pt_vtable;
    this_->m_license_plate = ++s_count;
    printf("PublicTransport::Ctor()%d\n", this_->m_license_plate);
}

void PTDtor(PublicTransport * const this_)
{
    this_->vptr = &pt_vtable;
    --s_count;
    printf("PublicTransport::Dtor()%d\n", this_->m_license_plate);
}

void PTCctor(PublicTransport * const this_, PublicTransport * const other_)
{
    this_->vptr = &pt_vtable;
    this_->m_license_plate = ++s_count;
    printf("PublicTransport::CCtor() %d\n", this_->m_license_plate);
    (void)other_;
}

int PTGetID(PublicTransport *const this_)
{
    return this_->m_license_plate;
}

void PTDisplay(PublicTransport *const this_)
{
    printf("PublicTransport::display(): %d\n",this_->m_license_plate);
}

/*--------------------------------- MiniBus -------------------------------*/
typedef struct mini_bus Minibus;
typedef void (*MB_vtfunc_t)(Minibus *const this_,int);

void MBCtor(Minibus *const  this_);
void MBDtor(Minibus *const this_);
void MBCctor(Minibus *const this_, Minibus *const other_);
void MBDisplay(Minibus *this_);
void MBWash(Minibus *this_, int minutes);

typedef struct MB_virtual_table
{
    PT_vtable_t vtbl;
    MB_vtfunc_t wash;

} MB_vtable_t;

struct mini_bus
{
    PublicTransport base;
    int m_numSeats;

};

static const MB_vtable_t mb_vtable = {{(PT_vtfunc_t)MBDtor, (PT_vtfunc_t)MBDisplay}, MBWash};

void MBCtor(Minibus *const this_)
{
    PTCtor((PublicTransport*const) this_);
    this_->base.vptr = (PT_vtable_t*)&mb_vtable;
    this_->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void MBDtor(Minibus *const this_)
{
    this_->base.vptr = (PT_vtable_t *)&mb_vtable;
    printf("Minibus::Dtor()\n");
    PTDtor(&this_->base);
}

void MBCctor(Minibus *const this_, Minibus *const other_)
{
    PTCctor((PublicTransport*const)this_,(PublicTransport*const)other_);
    this_->base.vptr = (PT_vtable_t *)&mb_vtable;
    this_->m_numSeats = other_->m_numSeats;
    printf("Minibus::CCtor() \n");
}

void MBDisplay(Minibus *this_)
{
    printf("Minibus::display() ID:%d", PTGetID((PublicTransport *const)this_));
    printf(" num seats:%d\n",this_->m_numSeats);
}
void MBWash(Minibus *this_, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes, PTGetID((PublicTransport *const)this_));
}

/*------------------------------------------- Taxi ------------------------------------------*/
typedef struct taxi Taxi; 

void TACtor(Taxi * const this_);
void TADtor(Taxi * const this_);
void TACctor(Taxi * const this_, Taxi * const other_);
void TADisplay(Taxi *this_);

typedef struct TA_virtual_table
{
    PT_vtable_t vtbl;

} TA_vtable_t;

struct taxi
{
    PublicTransport base;

};

static const TA_vtable_t ta_vtable = {{(PT_vtfunc_t)TADtor, (PT_vtfunc_t)TADisplay}};

void TACtor(Taxi *const this_)
{
    PTCtor((PublicTransport *const)this_);
    this_->base.vptr = (PT_vtable_t *)&ta_vtable;
 
    printf("Taxi::Ctor()\n");
}

void TADtor(Taxi *const this_)
{
    this_->base.vptr = (PT_vtable_t *)&ta_vtable;
    printf("Taxi::Dtor()\n");
    PTDtor((PublicTransport *const)this_);
}

void TACctor(Taxi *const this_, Taxi *const other_)
{
    PTCctor((PublicTransport*const)this_, (PublicTransport*const)other_);
    this_->base.vptr = (PT_vtable_t *)&ta_vtable;
    printf("Taxi::CCtor()\n");
}

void TADisplay(Taxi *this_)
{
    printf("Taxi::display() ID:%d\n", PTGetID((PublicTransport *const)this_));
}
/*----------------------------------- special Taxi --------------------------------------*/
typedef struct special_taxi SpecialTaxi;

void STACtor(SpecialTaxi * const this_);
void STADtor(SpecialTaxi * const this_);
void STACctor(SpecialTaxi * const this_, SpecialTaxi * const other_);
void STADisplay(SpecialTaxi *this_);

typedef struct STA_virtual_table
{
    TA_vtable_t vtbl;

} STA_vtable_t;

struct special_taxi
{
    Taxi base;

};

static const STA_vtable_t sta_vtable = {{{(PT_vtfunc_t)STADtor, (PT_vtfunc_t)STADisplay}}};

void STACtor(SpecialTaxi * const this_)
{
    TACtor((Taxi * const) this_);
    this_->base.base.vptr = (PT_vtable_t *)&sta_vtable;

    printf("SpecialTaxi::Ctor()\n");
}

void STADtor(SpecialTaxi *const this_)
{
    this_->base.base.vptr = (PT_vtable_t *)&sta_vtable;
    printf("SpecialTaxi::Dtor()\n");
    TADtor((Taxi *const)this_);
}

void STACctor(SpecialTaxi *const this_, SpecialTaxi *const other_)
{
    TACctor((Taxi *const)this_, (Taxi *const)other_);
    this_->base.base.vptr = (PT_vtable_t *)&ta_vtable;
    printf("SpecialTaxi::CCtor()\n");
}

void STADisplay(SpecialTaxi *this_)
{
    printf("SpecialTaxi::display() ID:%d", PTGetID((PublicTransport *const)this_));
}

/*------------------------------------ PublicConvoy -----------------------------------*/
typedef struct public_convoy PublicConvoy; 

void PCCtor(PublicConvoy *const this_);
void PCDtor(PublicConvoy *const this_);
void PCCctor(PublicConvoy *const this_, PublicConvoy *const other_);
void PCDisplay(PublicConvoy *this_);

typedef struct PC_virtual_table
{
    PT_vtable_t vtbl;

} PC_vtable_t;

struct public_convoy
{
    PublicTransport base;
    PublicTransport *m_pt1;
    PublicTransport *m_pt2;
    Minibus m_m;
    Taxi m_t;

};

static const PC_vtable_t pc_vtable = {{(PT_vtfunc_t)PCDtor, (PT_vtfunc_t)PCDisplay}};

void PCCtor(PublicConvoy *const this_)
{
    PTCtor((PublicTransport *const)this_);
    this_->base.vptr = (PT_vtable_t *)&pc_vtable;
    this_->m_pt1 = malloc(sizeof(Minibus));
    MBCtor((Minibus *const)this_->m_pt1);

    this_->m_pt2 = malloc(sizeof(PublicTransport));
    TACtor((Taxi *const)this_->m_pt2);

    MBCtor(&this_->m_m);
    TACtor(&this_->m_t);

    /* printf("PublicConvoy::Ctor()\n"); */
}

void PCDtor(PublicConvoy *const this_)
{
    this_->base.vptr = (PT_vtable_t *)&pc_vtable;

    this_->m_pt1->vptr->dtor((this_->m_pt1));
    free(this_->m_pt1);
    /* this_->m_pt1 = NULL; */
    this_->m_pt2->vptr->dtor((this_->m_pt2));
    free(this_->m_pt2);
    /* this_->m_pt2 = NULL; */
    TADtor(&this_->m_t);
    MBDtor(&this_->m_m);
    /* printf("PublicConvoy::Dtor()\n"); */
    PTDtor((PublicTransport *const)this_);
}

void PCCctor(PublicConvoy *const this_, PublicConvoy *const other_)
{
    PTCtor((PublicTransport *const)this_);
    this_->m_pt1 = malloc(sizeof(Minibus));
    MBCctor((Minibus *)this_->m_pt1, (Minibus *)other_->m_pt1);

    this_->m_pt2 = malloc(sizeof(Taxi));
    TACctor((Taxi *)this_->m_pt2, (Taxi *)other_->m_pt2);

    this_->base.vptr = (PT_vtable_t *)&pc_vtable;

    MBCctor(&this_->m_m,&other_->m_m);
    TACctor(&this_->m_t, &other_->m_t);

}
void PCDisplay(PublicConvoy *this_)
{
    ((MB_vtable_t *)this_->m_pt1->vptr)->vtbl.display(this_->m_pt1);
    ((TA_vtable_t *)this_->m_pt2->vptr)->vtbl.display(this_->m_pt2);
    MBDisplay(&this_->m_m);
    TADisplay(&this_->m_t);
}

/*------------------------------------ print info -------------------------------*/

void print_info_mn(Minibus *m)
{
    ((MB_vtable_t*)m->base.vptr)->wash(m,3);
}
void print_info_pt(PublicTransport *a)
{
    a->vptr->display(a);
}
void print_info()
{
    print_count();
}
void print_info_int(int i, PublicTransport *ret)
{
    Minibus m1;
    MBCtor(&m1);
    printf("print_info(int i)\n");
    MBDisplay(&m1);
    PTCctor(ret, &m1.base);
    MBDtor(&m1);
    (void)i;
}
/*----------------------------------- taxi display -------------------------------------*/

void Taxi_Display(Taxi *s)
{
    Taxi t_st;
    TACctor(&t_st, s);
    TADisplay(&t_st);
    TADtor(&t_st);
}
/*--------------------------------------------- main -----------------------------------*/
int main()
{
    int i = 0;
    Minibus m;
    PublicTransport p; 
    PublicTransport *array[3];
    PublicTransport arr2[3];
    Minibus m_a2;
    Taxi t_a2;
    Minibus m2;
    Minibus arr3[4];
    Taxi *arr4;
    SpecialTaxi st;

    PublicConvoy *ts1;
    PublicConvoy *ts2;

    /*****************************************/
    MBCtor(&m);
    print_info_mn(&m);
    print_info_int(3,&p);
    PTDisplay(&p);
    PTDtor(&p);
    array[0] = malloc(sizeof(Minibus));
    if (NULL == array[0])
    {
        exit(0);
    }
    MBCtor((Minibus*) array[0]);

    array[1] = malloc(sizeof(Taxi));
    if (NULL == array[1])
    {
        exit(0);
    }
    TACtor((Taxi *)array[1]);

    array[2] = malloc(sizeof(Minibus));
    if (NULL == array[2])
    {
        exit(0);
    }
    MBCtor((Minibus *)array[2]);


    for (i = 0; 3 > i; ++i)
    {
        array[i]->vptr->display(array[i]);
    }
    for (i = 0; 3 > i; ++i)
    {
        array[i]->vptr->dtor(array[i]);
        free(array[i]);
    }
    MBCtor(&m_a2);
    PTCctor(&arr2[0],&m_a2.base);
    MBDtor(&m_a2);

    TACtor(&t_a2);
    PTCctor(&arr2[1], &t_a2.base);
    TADtor(&t_a2);

    PTCtor(&arr2[2]);
    for (i = 0; 3 > i; ++i)
    {
        PTDisplay(&arr2[i]);
    }

    print_info_pt(&arr2[0]);

    print_count();

    MBCtor(&m2);
    print_count();

    for (i = 0; 4 > i; ++i)
    {
        MBCtor(&arr3[i]);
    }

    arr4 = (Taxi*)malloc(4 *sizeof(Taxi));
    if (NULL == arr4)
    {
        exit(0);
    }
    for (i = 0; 4 > i; ++i)
    {
        TACtor(&arr4[i]);
    }
    for(i = 3; 0 <=i; --i)
    {
        ((TA_vtable_t *)arr4[i].base.vptr)->vtbl.dtor((PublicTransport *)&arr4[i]);
    }
    free(arr4);

    printf("%d\n", MAX(1, 2));
    printf("%d\n", MAX(1, (int)2.0f));
    STACtor(&st);
    Taxi_Display((Taxi *) &st);

    ts1 = malloc(sizeof(PublicConvoy));
    if (NULL == ts1)
    {
        exit(0);
    }
    PCCtor(ts1);
    ts2 = malloc(sizeof(PublicConvoy));
    if (NULL == ts2)
    {
        exit(0);
    }
    PCCctor(ts2,ts1);
    ((PC_vtable_t *)ts1->base.vptr)->vtbl.display((PublicTransport *)ts1);
    ((PC_vtable_t *)ts2->base.vptr)->vtbl.display((PublicTransport *)ts2);

    ts1->base.vptr->dtor((PublicTransport*)ts1);
    free(ts1);

    ((PC_vtable_t *)ts2->base.vptr)->vtbl.display((PublicTransport *)ts2);
    
    ts2->base.vptr->dtor((PublicTransport *)ts2);
    free(ts2);

    STADtor(&st);
    for (i = 3; 0 <= i; --i)
    {
        MBDtor(&arr3[i]);
    }
    MBDtor(&m2);
    for (i = 2; 0 <= i; --i)
    {
        PTDtor(&arr2[i]);
    }
    MBDtor(&m);
    return 0;
}
