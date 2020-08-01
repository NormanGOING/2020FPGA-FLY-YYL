`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/07/19 17:30:53
// Design Name: 
// Module Name: Gyro_Demo_Verilog
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Gyro_Demo_Verilog(
    input i_clk,
    input i_rst,
    input i_en,
    input i_rx,
    inout i_iic_sda,
    output o_iic_scl,
    output o_tx,
    output reg[1:0]o_gpio_en=2'b11,
    output reg o_src_en=1'b0,
    inout  qspi_d0,
    inout  qspi_d1,
    inout  qspi_d2,
    inout  qspi_d3,
    input  I_qspi_cs,
    input  I_qspi_clk
    );
    
    //时钟
    wire clk_100MHz_system;
    
    //陀螺仪数据
    wire [15:0]temperature_data;
    wire [15:0]anglex_data;
    wire [15:0]angley_data;
    wire [15:0]anglez_data;
    wire [15:0]magx_data;
    wire [15:0]magy_data;
    wire [15:0]magz_data;
    
    //IIC的SDA线的三态节点
    wire iic_sda_i;
    wire iic_sda_o;
    wire iic_sda_t;
    
    //IIC驱动信号
    wire iic_busy;
    wire iic_mode;
    wire [7:0]slave_addr;
    wire [7:0]reg_addr_h;
    wire [7:0]reg_addr_l;
    wire [7:0]data_w;
    wire [7:0]data_r;
    wire iic_write;
    wire iic_read;
    
     //QSPI
    wire [31:0] addr;
    wire [7:0]  o_data;
    wire [7:0]  i_data;
    wire        o_valid;
    wire [31:0] addrb;
    wire [7:0]  dinb;
    wire [7:0]  doutb;
    wire        web;
    wire [31:0] addrc;
    wire [7:0]  dinc;
    wire [7:0]  doutc;
    wire        wec;
    wire [31:0] addrd;
    wire [7:0]  dind;
    wire [7:0]  doutd;
    wire        wed;
    
    //Tri-state gate
    IOBUF IIC_SDA_IOBUF
       (.I(iic_sda_o),
        .IO(i_iic_sda),
        .O(iic_sda_i),
        .T(~iic_sda_t));
        
    //系统时钟
    clk_wiz_0 System_Clock(.clk_out1(clk_100MHz_system),.clk_in1(i_clk));
    
    //陀螺仪驱动
    Driver_Gyro Gyro_0(
        .i_clk(clk_100MHz_system),
        .i_rst(i_rst),
        .i_en(i_en),
        .i_iic_busy(iic_busy),
        .i_data_mode(3'b111),
        .i_data(data_r),
        .o_reg_mode(iic_mode),
        .o_slave_addr(slave_addr),
        .o_reg_addr_h(reg_addr_h),
        .o_reg_addr_l(reg_addr_l),  
        .o_data_w(data_w),      
        .o_temperature_data(temperature_data),
        .o_anglex_data(anglex_data),
        .o_angley_data(angley_data),
        .o_anglez_data(anglez_data),
        .o_magx_data(magx_data),
        .o_magy_data(magy_data),
        .o_magz_data(magz_data),
        .o_iic_write(iic_write),
        .o_iic_read(iic_read)
    );

    //UART
    UART_Ctrl UART0_Ctrl(
        .i_clk(clk_100MHz_system),
        .i_rst(i_rst),
        .i_baudrate(31'd9600),
        .i_rx(i_rx),
        .i_temperature_data(temperature_data),
        .i_anglex_data(anglex_data),
        .i_angley_data(angley_data),
        .i_anglez_data(anglez_data),
        .i_magx_data(magx_data),
        .i_magy_data(magy_data),
        .i_magz_data(magz_data),
        .o_tx(o_tx),
        .o_rx_data(),
        .o_rx_rq()
    );
    
    //IIC驱动
    Driver_IIC IIC_0(
        .i_clk(clk_100MHz_system),
        .i_rst(i_rst),
        .i_iic_sda(iic_sda_i),
        .i_iic_write(iic_write),                //IIC写信号,上升沿有效
        .i_iic_read(iic_read),                  //IIC读信号,上升沿有效
        .i_iic_mode(iic_mode),                  //IIC模式,1代表双地址位,0代表单地址位,低位地址有效
        .i_slave_addr(slave_addr),              //IIC从机地址
        .i_reg_addr_h(reg_addr_h),              //寄存器地址,高8位
        .i_reg_addr_l(reg_addr_l),              //寄存器地址,低8位
        .i_data_w(data_w),                      //需要传输的数据
        .o_data_r(data_r),                      //IIC读到的数据
        .o_iic_busy(iic_busy),                  //IIC忙信号,在工作时忙,低电平忙
        .o_iic_scl(o_iic_scl),                  //IIC时钟线
        .o_sda_dir(iic_sda_t),                  //IIC数据线方向,1代表输出
        .o_iic_sda(iic_sda_o)                   //IIC数据线
    );
    qspi_slave u_qspi_slave(
    .I_qspi_clk  (I_qspi_clk)  , 
    .I_qspi_cs   (I_qspi_cs)  , 
    .IO_qspi_io0 (qspi_d0)  ,
    .IO_qspi_io1 (qspi_d1)  ,
    .IO_qspi_io2 (qspi_d2)  , 
    .IO_qspi_io3 (qspi_d3)  , 
    .o_addr      (addr)    ,
    .o_data      (o_data)  ,
    .i_data      (i_data)  ,
    .o_valid     (o_valid)
    );

blk_mem_gen_0 u_blk_mem_gen_0(
    .addra(addr),
    .clka(I_qspi_clk),
    .dina(o_data),
    .douta(i_data),
    .wea(o_valid),
    .addrb(addrb),
    .clkb(clk_100MHz_system),
    .dinb(dinb),
    .doutb(doutb),
    .web(web)
);

//Gyro     
Gyro2ram u_Gyro2ram(
    .clk(clk_100MHz_system),
    .rst_n(1),
    //Gyro
    .Temp_Data(temperature_data),
    .Gyro_Data_X(anglex_data),
    .Gyro_Data_Y(angley_data),
    .Gyro_Data_Z(anglez_data),
    .Mag_Data_X(magx_data),
    .Mag_Data_Y(magx_data),
    .Mag_Data_Z(magx_data),
    //TEST_DATA
//    .Temp_Data(16'h3231),
//    .Gyro_Data_X(16'h3433),
//    .Gyro_Data_Y(16'h3635),
//    .Gyro_Data_Z(16'h3837),
//    .Mag_Data_X(16'h3A39),
//    .Mag_Data_Y(16'h3C3B),
//    .Mag_Data_Z(16'h3E3D),
    //RAM 
    .addr(addrb),
    .data_in(doutb),
    .data_out(dinb),
    .wen(web)
    ); 
endmodule
