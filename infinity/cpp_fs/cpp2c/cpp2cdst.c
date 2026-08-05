#include <stdio.h>      /* printf(), puts() */
#include <stdlib.h>     /* malloc(), free() */

#include "cpp2c.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define NUM_OF_SEAT (20)
enum Switcher {PT = 1,VOID,MN,INT};

/*---------------------- structs --------------------------*/
/*-------- public transporter ---------*/
typedef struct public_transport_vtable
{
    PT_dtor_t pt_dtor;
    PT_dtordl_t pt_dtordl;
    PT_display_t pt_display;
    
}public_transport_vtable_t;

struct PublicTransport
{
    const public_transport_vtable_t *v_ptr;

    PT_print_count_t p_count;
    PT_get_id_t get_id;

    int m_license_plate;

};
/*------------- minibus ---------------*/
typedef struct minibus_vtable
{
    MB_dtor_t mb_dtor;
    MB_dtordl_t mb_dtordl;
    MB_display_t mb_display;
    MB_wash_t mb_wash;

} minibus_vtable_t;

struct MiniBus
{
    PublicTransport_t base;

    int m_numSeats;
};
/*------------- taxi ---------------*/
typedef struct taxi_vtable
{
    TA_dtor_t ta_dtor;
    TA_dtordl_t ta_dtordl;
    TA_display_t ta_display;

} taxi_vtable_t;

struct Taxi
{
    PublicTransport_t base;
};

/*------------- specialtaxi ---------------*/
typedef struct specialtaxi_vtable
{
    STA_dtor_t sta_dtor;
    STA_dtordl_t sta_dtordl;
    STA_display_t sta_display;

} specialtaxi_vtable_t;

struct SpecialTaxi
{
    Taxi_t base;
};
/*------------------- static variables ------------------------*/ 


static int s_count = 0;
static const public_transport_vtable_t pt_vtable = {PTDtor,PTDtorDl, PTDisplay};
static const minibus_vtable_t mb_vtable = {MBDtor,MBDtorDl, MBDisplay, Wash};
static const taxi_vtable_t ta_vtable = {TADtor, TADtorDl, TADisplay};
static const specialtaxi_vtable_t sta_vtable = {STADtor, STADtorDl, STADisplay};

/*--------------------function definitions --------------------*/

/*-------- public transporter ---------*/

PublicTransport_t PTCtor(PublicTransport_t *this_)
{
    this_->v_ptr = &pt_vtable;
    this_->p_count = PTPrintCount;
    this_->get_id = PTGetId;
    this_->m_license_plate = ++s_count;
    printf("PublicTransport_t::Ctor() %d\n", this_->m_license_plate);
    return *this_;
}

PublicTransport_t *PTCtorAl()
{
    PublicTransport_t *new_pt = malloc(sizeof(PublicTransport_t));
    /* new_pt->v_ptr = &pt_vtable;
    new_pt->p_count = PTPrintCount;
    new_pt->get_id = PTGetId;
    new_pt->m_license_plate = ++s_count;
    printf("PublicTransport_t::Ctor() %d\n", new_pt->m_license_plate); */
    *new_pt = PTCtor(new_pt);
    return new_pt;
}

void PTDtor(PublicTransport_t* this_)
{
    this_->v_ptr = (public_transport_vtable_t*) &pt_vtable;
    --s_count;
    printf("PublicTransport_t::Dtor() %d\n", this_->m_license_plate);
}

void PTDtorDl(PublicTransport_t *this_)
{
    PTDtor(this_);
    free(this_);
    this_ = NULL;
}

PublicTransport_t PTCCtor(PublicTransport_t *this_, const PublicTransport_t *other_)
{

    this_->v_ptr = &pt_vtable;
    this_->p_count = other_->p_count;
    this_->get_id = other_->get_id;
    this_->m_license_plate = ++s_count;

    printf("PublicTransport_t::CCtor() %d\n", this_->m_license_plate);
    return *this_;
}

void PTPrintCount(void)
{
    printf("s_count: %d \n", s_count);
}

void PTDisplay(PublicTransport_t* this_)
{
    printf("PublicTransport_t::display(): %d\n", this_->m_license_plate);
}

int PTGetId(PublicTransport_t* this_)
{
    return this_->m_license_plate;
}

/*------------- minibus ---------------*/
MiniBus_t MBCtor(MiniBus_t *this_)
{
    this_->base = PTCtor(&this_->base);
    this_->base.v_ptr = (public_transport_vtable_t *)&mb_vtable;
    this_->m_numSeats = NUM_OF_SEAT;
    printf("Minibus::Ctor()\n");
    return *this_;
}
MiniBus_t *MBCtorAl()
{
    MiniBus_t *new_mb = malloc(sizeof(MiniBus_t));
    /* new_mb->base = PTCtor(&new_mb->base);
    new_mb->base.v_ptr = (public_transport_vtable_t *)&mb_vtable;
    new_mb->m_numSeats = NUM_OF_SEAT;
    printf("Minibus::Ctor()\n"); */
    *new_mb = MBCtor(new_mb);
    return new_mb;
}

void MBDtor(MiniBus_t *this_)
{
    printf("Minibus::Dtor()\n");
    PTDtor(&this_->base);

}

void MBDtorDl(MiniBus_t *this_)
{
    MBDtor(this_);
    free(this_);
    this_ = NULL;
}

MiniBus_t MBCCtor(MiniBus_t *this_, const MiniBus_t *other_)
{

    PTCCtor(&this_->base,&other_->base);
    this_->base.v_ptr = (public_transport_vtable_t *)&mb_vtable;
    this_->m_numSeats = other_->m_numSeats;
    printf("Minibus::CCtor() \n");
    return *this_;
}

void MBDisplay(MiniBus_t* this_)
{
    printf("Minibus::display() ID: %d ", this_->base.get_id(&this_->base));
    printf("num seats:%d\n",this_->m_numSeats);
}
void Wash(MiniBus_t* this_, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n",minutes,this_->base.get_id(&this_->base));
}

/*------------- taxi ---------------*/
Taxi_t TACtor(Taxi_t *this_)
{
    this_->base = PTCtor(&this_->base);
    this_->base.v_ptr = (public_transport_vtable_t *)&ta_vtable;
    printf("Taxi::Ctor()\n");
    return *this_;
}
Taxi_t *TACtorAl()
{
    Taxi_t *new_ta = malloc(sizeof(Taxi_t));
    /* new_ta->base = PTCtor(&new_ta->base);
    new_ta->base.v_ptr = (public_transport_vtable_t *)&ta_vtable;
    printf("Taxi::Ctor()\n"); */
    *new_ta = TACtor(new_ta);
    return new_ta;
}

void TADtor(Taxi_t *this_)
{
    printf("Taxi::Dtor()\n");
    PTDtor(&this_->base);
}

void TADtorDl(Taxi_t *this_)
{
    TADtor(this_);
    free(this_);
    this_ = NULL;
}

Taxi_t TACCtor(Taxi_t *this_, const Taxi_t *other_)
{
    PTCCtor(&this_->base, &other_->base);
    this_->base.v_ptr = (public_transport_vtable_t *)&ta_vtable;
    printf("Taxi::CCtor()\n");
    return *this_;
}
void TADisplay(Taxi_t *this_)
{
    printf("Taxi::display() ID: %d\n", this_->base.get_id(&this_->base));
}

/*------------- specialtaxi ---------------*/
SpecialTaxi_t STACtor(SpecialTaxi_t *this_)
{
    this_->base = TACtor(&this_->base);
    this_->base.base.v_ptr = (public_transport_vtable_t *)&sta_vtable;
    printf("SpecialTaxi::Ctor()\n");
    return *this_;
}
SpecialTaxi_t *STACtorAl()
{
    SpecialTaxi_t *new_sta = malloc(sizeof(SpecialTaxi_t));
    /* new_ta->base = TACtor(&new_ta->base);
    new_ta->base.base.v_ptr = (public_transport_vtable_t *)&sta_vtable;
    printf("SpecialTaxi::Ctor()\n"); */
    *new_sta = STACtor(new_sta);
    return new_sta;
}

void STADtor(SpecialTaxi_t *this_)
{
    printf("SpecialTaxi::Dtor()\n");
    TADtor(&this_->base);
    
}

void STADtorDl(SpecialTaxi_t *this_)
{
    STADtor(this_);
    free(this_);
    this_ = NULL;
}

SpecialTaxi_t STACCtor(SpecialTaxi_t *this_, const SpecialTaxi_t *other_)
{
    TACCtor(&this_->base,&other_->base);
    this_->base.base.v_ptr = (public_transport_vtable_t *)&sta_vtable;
    printf("SpecialTaxi::CCtor()\n");
    return *this_;
}
void STADisplay(SpecialTaxi_t *this_)
{
    printf("SpecialTaxi::display() ID: %d\n", this_->base.base.get_id(&this_->base.base));
}

/*--------------- printinfo --------------*/
void PrintInfo(void *arg, int switcher, PublicTransport_t *ret)
{
    PublicTransport_t* pt = NULL;
    MiniBus_t * mb = NULL;
    minibus_vtable_t* mb_vptr = NULL;
    MiniBus_t m1;

    switch (switcher)
    {
    case PT:
        pt = (PublicTransport_t*)arg;
        pt->v_ptr->pt_display(pt);
        break;
    case VOID:
        PTPrintCount();
        break;
    case MN:
        mb = (MiniBus_t*)arg;
        mb_vptr = (minibus_vtable_t*)(mb->base.v_ptr);
        mb_vptr->mb_wash(mb,3);
        
        break;
    case INT:
        m1 = MBCtor(&m1);
        printf("print_info(int i)\n");
        /* m1.base.v_ptr->pt_display(&m1.base); */
        MBDisplay(&m1);
        PTCCtor(ret, &m1.base);
        /* mb_vptr = (minibus_vtable_t*)m1.base.v_ptr;
        mb_vptr->mb_dtor(&m1); */
        MBDtor(&m1);
    }

}
void Taxi_Display(Taxi_t s)
{
    Taxi_t t_st = TACCtor(&t_st, &s);
    TADisplay(&t_st);
    TADtor(&t_st);
}

/***************************************** main ***************************************/

int main(int argc, char **argv)
{
    minibus_vtable_t *mb_vptr = NULL;
    taxi_vtable_t *ta_vptr = NULL;
    specialtaxi_vtable_t *sta_vptr = NULL;
    MiniBus_t m1 = MBCtor(&m1);
    PublicTransport_t p1;
    PublicTransport_t *array[3];
    PublicTransport_t arr2[3];
    MiniBus_t *m1_a1 = NULL;
    Taxi_t *t_a1 = NULL;
    MiniBus_t *m2_a1 = NULL;

    MiniBus_t m1_a2;
    Taxi_t t1_a2;

    int i = 0;
    MiniBus_t m2;
    MiniBus_t arr3[4];
    Taxi_t *arr4;

    SpecialTaxi_t st;

    PrintInfo(&m1,MN,NULL);
    PrintInfo((void*)3,INT,&p1);
    PTDisplay(&p1);
    p1.v_ptr->pt_dtor(&p1);


    array[0] = (PublicTransport_t *)MBCtorAl();
    array[1] = (PublicTransport_t *)TACtorAl();
    array[2] = (PublicTransport_t *)MBCtorAl();

    for ( i = 0; 3 > i; ++i)
    {
        array[i]->v_ptr->pt_display(array[i]);
    }

    for (i = 0; 3 > i; ++i)
    {
        array[i]->v_ptr->pt_dtordl(array[i]);
    }

    m1_a2 = MBCtor(&m1_a2);
    arr2[0] = PTCCtor(&arr2[0],&m1_a2.base);
    mb_vptr = (minibus_vtable_t *)m1_a2.base.v_ptr;
    mb_vptr->mb_dtor(&m1_a2);

    t1_a2 = TACtor(&t1_a2);
    arr2[1] = PTCCtor(&arr2[1],&t1_a2.base);
    ta_vptr = (taxi_vtable_t *)t1_a2.base.v_ptr;
    ta_vptr->ta_dtor(&t1_a2);

    arr2[2] = PTCtor(&arr2[2]);
    
    for (i = 0; 3 > i; ++i)
    {
        arr2[i].v_ptr->pt_display(&arr2[i]);
    }
    PrintInfo((void*)&arr2[0],PT,NULL);

    PTPrintCount();
    m2 = MBCtor(&m2);
    mb_vptr = (minibus_vtable_t *)m2.base.v_ptr;
    m2.base.p_count();
    for(i = 0; 4 > i; ++i)
    {
        arr3[i] = MBCtor(&arr3[i]);
    }

    arr4 = malloc(sizeof(Taxi_t) * 4);
    for (i = 0; 4 > i; ++i)
    {
        arr4[i] = TACtor(&arr4[i]);
    }
    for (i = 3; 0 <= i; --i)
    {
        ta_vptr = (taxi_vtable_t *)arr4[i].base.v_ptr;
        ta_vptr->ta_dtor(&arr4[i]);
    }
    free(arr4);
    arr4 = NULL;
    printf("%d\n", MAX(1,2));
    printf("%d\n", MAX(1,(int)2.0f));

    st = STACtor(&st);
    Taxi_Display(st.base);

    

    sta_vptr = (specialtaxi_vtable_t *)st.base.base.v_ptr;
    sta_vptr->sta_dtor(&st);
    for (i = 3; 0 <= i; --i)
    {
        mb_vptr = (minibus_vtable_t *)arr3[i].base.v_ptr;
        mb_vptr->mb_dtor(&arr3[i]);
    }
    mb_vptr = (minibus_vtable_t *)m2.base.v_ptr;
    mb_vptr->mb_dtor(&m2);
    for (i = 2; 0 <= i; --i)
    {
        arr2[i].v_ptr->pt_dtor(&arr2[i]);
    }
    mb_vptr = (minibus_vtable_t *)m1.base.v_ptr;
    mb_vptr->mb_dtor(&m1);

    (void)argc;
    (void)argv;
    return 0;
}



