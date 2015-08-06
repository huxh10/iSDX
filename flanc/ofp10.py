#  Author:
#  Rudiger Birkner (Networked Systems Group ETH Zurich)

class FlowMod():
    def __init__(self, origin, flow_mod):
        self.mod_types = ["insert", "remove"]
        self.rule_types = ["inbound", "outbound", "main"]
        
        self.config
        self.parser

        self.mod_type = None
        self.rule_type = None

        self.origin = None
        self.datapath = None
        self.priority = None
        self.cookie = None
        self.matches = {}
	self.actions = []

        validate_flow_mod(origin, flow_mod)

    def get_flow_mod_msg():
        return self.flow_mod

    def validate_flow_mod(flow_mod):
        if "origin" in flow_mod:
            self.origin = int(flow_mod["origin"])
            if "id" in flow_mod:
                self.cookie = int('{0:032b}'.format(self.origin)+'{0:032b}'.format(int(flow_mod["id"])),2)
                if ("mod_type" in flow_mod and flow["mod_type"] in self.mod_types):
                    self.mod_type = flow_mod["mod_type"]
                    if ("rule_type" in flow_mod and flow["rule_type"] in self.rule_types):
                        self.rule_type = flow_mod["rule_type"]
                        if ("priority" in flow_mod):
                            self.priority = flow_mod["priority"]
                            if "match" in flow_mod:
                                self.match = validate_match(flow_mod["match"])
                            if "action" in flow_mod:
                                self.actions = validate_action(flow_mod["action"])

    def validate_match(matches):
        validated_matches = {}

        for match, value in matches.iteritems():

            if match == "eth_type":
                validated_matches[match] = value
            elif match == "in_port":
                validated_matches[match] = value
            elif match == "arp_tpa":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_ARP
            elif match == "eth_dst":
                validated_matches[match] = value
            elif match == "eth_src":
                validated_matches[match] = value
            elif match == "ipv4_src":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
            elif match == "ipv4_dst":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
            elif match == "tcp_src":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
                if "ip_proto" not in validated_matches:
                    validated_matches["ip_proto"] = inet.IPPROTO_TCP
            elif match == "tcp_dst":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
                if "ip_proto" not in validated_matches:
                    validated_matches["ip_proto"] = inet.IPPROTO_TCP
            elif match == "udp_src":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
                if "ip_proto" not in validated_matches:
                    validated_matches["ip_proto"] = inet.IPPROTO_UDP
            elif match == "udp_dst":
                validated_matches[match] = value
                if "eth_type" not in validated_matches:
                    validated_matches["eth_type"] = ether.ETH_TYPE_IP
                if "ip_proto" not in validated_matches:
                    validated_matches["ip_proto"] = inet.IPPROTO_UDP
        return validated_matches

    def make_actions():
        temp_actions = []

        for action, value in actions.iteritems():
            if action == "fwd":
                for port in value:
                    if isinstance( value, int ) or value.isdigit():
                        temp_actions.append(self.parser.OFPActionOutput(int(value)))
                    else:
                        temp_actions.append(self.parser.OFPActionOutput(self.config.datapath_ports[self.rule_type][value]))
            elif action == "set_eth_src":
                temp_actions.append(self.parser.OFPActionSetDlDst(value))
            elif action == "set_eth_dst":
                temp_actions.append(self.parser.OFPActionSetDlDst(value))

        return temp_actions

    def validate_action(actions):
        validated_actions = {}

        for action, value in actions.iteritems():
            if action == "fwd":
                validated_actions[action] = int(value)
            elif action == "set_eth_src":
                validated_actions[action] = value
            elif action == "set_eth_dst":
                validated_actions[action] = value
        return validated_actions

    def get_flow_mod(config):
        self.config = config
        self.parser = config.parser

        match = self.parser.OFPMatch(**self.matches)

        datapath = self.config.datapaths[rule_type]

        actions = make_actions()

        if self.mod_type == "insert":
            instructions = make_instructions()
            return self.parser.OFPFlowMod(datapath=datapath, 
                                          match=match, 
                                          cookie=self.cookie, 
                                          command=self.config.ofproto.OFPFC_ADD, 
                                          priority=self.priority, actions=actions)
        else:
            return self.parser.OFPFlowMod(datapath=datapath, 
                                          match=match, 
                                          cookie=self.cookie,
                                          command=self.config.ofproto.OFPFC_DELETE, 
                                          out_group=self.config.ofproto.OFPG_ANY, 
                                          out_port=self.config.ofproto.OFPP_ANY)

    def get_dst_dp():
        return rule_type 