def process_bbq_order(order):
    order_list = order.split("#")
    order_format = []
    for param in order_list:
        order_format.append(param.split("|"))

    print(order_list,order_format)

process_bbq_order("QxMeat(preference)|Sides:Side1,Side2|Special:Instruction#QxMeat(preference)|Sides:Side1,Side2|Special:Instruction")
