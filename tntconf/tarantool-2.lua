box.cfg { listen=3302, log_level=6 }
s = box.schema.space.create('test')
i = s:create_index('primary', {parts = {1, 'STR'}})
box.schema.user.grant('guest', 'execute,read,write', 'universe')
json = require('json')
uuid = require('uuid')
