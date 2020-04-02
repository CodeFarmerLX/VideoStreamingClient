#ifndef PACKAGE_H
#define PACKAGE_H

enum dataType{
    TYPE_REGIST     =  1,//注册行为
    TYPE_REGIST_FAIL,//注册失败（用户名已存在）
    TYPE_REGIST_SUCCESS,//注册成功

    TYPE_LOGIN,//登录行为
    TYPE_LOGIN_FAIL,//登录失败
    TYPE_LOGIN_FAIL_NAME,//登录失败（用户名不存在）
    TYPE_LOGIN_FAIL_PW,//登陆失败（密码不正确）
    TYPE_LOGIN_FAIL_ONLINE,//登录失败（用户已在线）
    TYPE_LOGIN_SUCCESS,//登录成功

    TYPE_LOGOUT,//登出行为
    TYPE_LOGOUT_BACK,//登出行为（后返回登录界面）
    TYPE_LOGOUT_FAIL,//登出失败
    TYPE_LOGOUT_SUCCESS,//登出成功(退出程序)
    TYPE_LOGOUT_SUCCESS_BACK,//登出成功（退出上一个界面）

    TYPE_FIND_PW,//找回密码行为
    TYPE_FIND_PW_FAIL_NAME,//找回失败（名字不存在）
    TYPE_FIND_PW_FAIL_PHONE,//找回失败（电话号码不正确）
    TYPE_FIND_PW_SUCCESS,//找回成功

    TYPE_UPDATE_ROOMS,//更新房间行为
    TYPE_UPDATE_ROOMS_CLEAR_TEXT,//刷新所有屏幕时先清空房间列表内容
    TYPE_UPDATE_ROOMS_ALL,//刷新所有套接字的房间
    TYPE_UPDATE_ROOMS_FAIL,//更新失败
    TYPE_UPDATE_ROOMS_SUCCESS,//更新成功
    TYPE_UPDATE_ROOMS_SUCCESS_NO_ROOM,//更新成功(没有房间)

    TYPE_JOIN_ROOM,//加入房间行为
    TYPE_JOIN_ROOM_FAIL,//加入房间失败
    TYPE_JOIN_ROOM_SUCCESS,//加入房间成功

    TYPE_EXIT_ROOM,//退出房间行为
    TYPE_EXIT_ROOM_SUCCESS,//退出房间行为成功
    TYPE_EXIT_ROOM_SUCCESS_UPDATE_USR,//退出房间行为成功(刷新房间内用户列表)
    TYPE_EXIT_ROOM_SUCCESS_CLEAR_USR,//退出房间行为成功(用户列表清空)
    TYPE_EXIT_ROOM_FAIL,//退出房间失败

    TYPE_CLOSE_ROOM,//关闭房间行为
    TYPE_CLOSE_ROOM_SUCCESS_FOR_ROOMER,//关闭房间成功向房主提示
    TYPE_CLOSE_ROOM_SUCCESS_FOR_USR,//关闭房间成功向房间内游客提示

    TYPE_SENT_TEXT_IN_ROOM,//在房间里群发消息
    TYPE_GET_TEXT_IN_ROOM,//在房间里里收到消息

    TYPE_RECHARGE,//冲值行为
    TYPE_RECHARGE_FAIL,//充值失败
    TYPE_RECHARGE_SUCCESS,//充值成功

    TYPE_UP_USR_INFO,//更新用户信息
    TYPE_BACK_USR_INFO,//返回用户信息

    TYPE_CHANGE_PW,//更改密码行为
    TYPE_CHANGE_PW_FAIL,//更改密码失败
    TYPE_CHANGE_PW_SUCCESS,//更改成功

    TYPE_CHANGE_PHONE,//更改电话号码
    TYPE_CHANGE_PHONE_FAIL,//更改电话失败
    TYPE_CHANGE_PHONE_SUCCESS,//更改成功

    TYPE_SENT_FLOWER,//送鲜花礼物
    TYPE_SENT_FLOWER_FAIL,//送礼物失败
    TYPE_SENT_FLOWER_SUCCESS_USR,//送礼物成功
    TYPE_SENT_FLOWER_SUCCESS_ROOMER,//送礼物成功

    TYPE_CREATE_ROOM,//创建房间行为
    TYPE_CREATE_ROOM_FAIL,//创建失败（钱不够）
    TYPE_CREATE_ROOM_FAIL_ID_NUMBER,//创建失败（身份证号吗不存在，或与姓名不符）
    TYPE_CREATE_ROOM_SUCCESS,//创建成功
    //...
};


typedef struct{
    dataType    type;
    int         id;
    char        name[20];
    char        roomName[100];
    char        pwd[20];
    int         tmoney;
    int         money;
    char        phone[12];
    int         point;
    char        idNumber[20];
    char        roomIpAddress[20];
    unsigned short  roomPort;
    int         roomId;
    char        message[160];

}pack_t;


#endif // PACKAGE_H
