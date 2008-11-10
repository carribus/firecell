-- Item Selection
select  	items.item_id, items.item_name, types.itemtype_name, types.itemtype_table, 
		items.object_id, items.description, items.min_level, items.max_level, items.npc_value
from 		fc_items items
inner join 	fc_itemtypes types
on 		types.itemtype_id = items.itemtype_id

-- Processor Selection
SELECT 		i.item_id, i.item_name, it.itemtype_id, it.itemtype_name, i.description, i.min_level, i.max_level, 
		i.npc_value, p.processor_id, p.core_count, p.core_speed
FROM 		fc_items i
INNER JOIN	fc_itemtypes it
	ON 	it.itemtype_id = i.itemtype_id
INNER JOIN	fc_processors p
	ON 	p.processor_id = i.object_id
WHERE		i.itemtype_id = 1

-- OS Selection

SELECT 		i.item_id, i.item_name, it.itemtype_id, it.itemtype_name, i.description, i.min_level, i.max_level, 
		i.npc_value, o.os_id, o.oskernel_id, osk.name
FROM 		fc_items i
INNER JOIN	fc_itemtypes it
	ON 	it.itemtype_id = i.itemtype_id
INNER JOIN	fc_operatingsystems o
	ON 	o.os_id = i.object_id
INNER JOIN 	fc_oskernels osk
	ON 	osk.oskernel_id = o.oskernel_id
WHERE		i.itemtype_id = 2
