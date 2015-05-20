macroScript Free_Camera category:"Material" tooltip:"Change dds to png"
(
	/*for i in selection do*/
	for m in sceneMaterials do
	(
		local i = m
		for j in i.maps do
		(
			if j!=undefined  do
			(
				if hasProperty j "filename" and j.filename != undefined then
				(
					local before = j.filename + " ==> "
					j.filename=replace j.filename (j.filename.count-2) 3 "tga"
					print (before + j.filename)
				)
				else
				(
					if hasProperty j "normal_map" and j.normal_map != undefined do
					(
						local before =  j.normal_map.filename + " ==> "
						j.normal_map.filename=replace j.normal_map.filename (j.normal_map.filename.count-2) 3 "tga"
						print (before + j.normal_map.filename)
					)
					if hasProperty j "bump_map" and j.bump_map != undefined do
					(
						local before =  j.bump_map.filename + " ==> "
						j.bump_map.filename=replace j.bump_map.filename (j.bump_map.filename.count-2) 3 "tga"
						print (before  + j.bump_map.filename)
					)
				)
			)
				
		)
		
		
		/*showProperties i.material*/
		/*
		i.material.diffuseMap.filename=replace i.material.diffuseMap.filename (i.material.diffuseMap.filename.count-2) 3 "tga"
		
		
		
		
		

	for i in selection do i.material.specularMap.filename=replace i.material.specularMap.filename (i.material.specularMap.filename.count-2) 3 "tga"
		
	for i in selection do i.material.glossinessMap.filename=replace i.material.glossinessMap.filename (i.material.glossinessMap.filename.count-2) 3 "tga"
	for i in selection do i.material.selfIllumMap.filename=replace i.material.glossinessMap.filename (i.material.selfIllumMap.filename.count-2) 3 "tga"
	for i in selection do i.material.opacityMap.filename=replace i.material.glossinessMap.filename (i.material.opacityMap.filename.count-2) 3 "tga"
	for i in selection do i.material.filterMap.filename=replace i.material.glossinessMap.filename (i.material.filterMap.filename.count-2) 3 "tga"
	for i in selection do i.material.bumpMap.filename=replace i.material.glossinessMap.filename (i.material.bumpMap.filename.count-2) 3 "tga"
	for i in selection do i.material.reflectionMap.filename=replace i.material.glossinessMap.filename (i.material.reflectionMap.filename.count-2) 3 "tga"
	for i in selection do i.material.refractionMap.filename=replace i.material.glossinessMap.filename (i.material.refractionMap.filename.count-2) 3 "tga"
		*/
	)
	
)