//class Transform extends Component { 
//	// the parent transform of this transform 
//	// if it is null then the parent transform 
//	// is the world coordinate system 
//	private Transform parent; 
//	// all of the transforms that have this 
//	// transform set as their parent 
//	private Transform[] children; 
//	// the position relative to the parent transform 
//	private Vector2 localPosition = new Vector2(0.0f, 0.0f); 
//	// rotation relative to the parent 
//	private float localRotation = 0.0f; 
//	// scale relative to the parent 
//	private Vector2 localScale = new Vector2(1.0f, 1.0f); 
//	// specifies if the localToWorldTransform 
//	// needs to be recalulated 
//	private bool isDirty = false; 
//	// the transform that converts local coordinates 
//	// to world coordinates 
//	private Matrix localToWorldMatrix = Matrix.identity; 
//	// specifies if the worldToLocalMatrix 
//	// needs to be recalculated 
//	private bool isInverseDirty = false; 
//	// the transform that converts world cooridnates 
//	// to local coordinates 
//	private Matrix worldToLocalMatrix = Matrix.identity; 
//	/* * Whenever any change happens that changes the localToWorldMatrix * this should be called. That way the next time localToWorldMatrix * is requested it will be recalculated */ 
//	
//	private void setDirty() { 
//		// only update dirty boolean if it isn't already dirty 
//		if (!isDirty) { 
//			isDirty = true; 
//			isInverseDirty = true; 
//			// set all children to be dirty since any modification 
//			// of a parent transform also effects its children's 
//			// localToWorldTransform 
//			foreach (Transform child in children)  { 
//				child.setDirty(); 
//			} 
//		} 
//	} 
//	
//	// change the parent transform. 
//	// setting it to null makes the 
//	// transform a child of world coordinates 
//	public void setParent(Transform value) { 
//		// remove this from the previous parent 
//		if (parent != null) { 
//			parent.children.remove(this); 
//		} 
//		// assign new parent 
//		parent = value; 
//		// add this to new parent 
//		if (parent) { 
//			parent.children.add(this); 
//		} 
//		// changes parents effects 
//		// the world position 
//		setDirty(); 
//	} 
//	
//	public Transform getParent() { 
//		return parent; 
//	} 
//	
//	// calculates the transform matrix that converts 
//	// from local coordinates to the coordinate space 
//	// of the parent transform 
//	public Matrix calculateLocalToParentMatrix() { 
//		// Matrix.translate creates a translation matrix 
//		// that shifts by (localPosition.x, localPosition.y) 
//		// Matrix.rotate rotates by localRotation radians 
//		// Matrix.scale scales by a factor of (localScale.x, localScale.y) 
//		// These are the basic transforms that are described previously 
//		// in this article 
//		return Matrix.translate(localPosition) * Matrix.rotate(localRotation) * Matrix.scale(localScale); 
//	} 
//	
//	// gets the matrix that converts from local 
//	// coordinates to world coordinates 
//	public Matrix getLocalToWorldMatrix() { 
//		// if the dirty flag is set, the the 
//		// localToWorldMatrix is out of date 
//		// and needs to be reclaculated 
//		if (isDirty) { 
//			if (parent == null) { 
//				// if the parent is null then the parent is 
//				// the world so the localToWorldMatrix 
//				// is the same as local to parent matrix 
//				localToWorldMatrix = calculateLocalToParentMatrix(); 
//			} else { 
//				// if there is a parent, then the localToWorldMatrix 
//				// is calcualted recursively using the parent's localToWorldMatrix 
//				// concatenated with the local to parent matrix 
//				localToWorldMatrix = parent.getLocalToWorldMatrix() * calculateLocalToParentMatrix(); 
//			} 
//			// clear the dirty flag since the 
//			// matrix is now up to date 
//			isDirty = false; 
//		} 
//		return localToWorldMatrix; 
//	} 
//	
//	public Matrix getWorldToLocalMatrix() { 
//		if (isInverseDirty) { 
//			// the inverse is out of date 
//			// so it needs to be updated 
//			// the worldToLocalMatrix is the inverse of 
//			// the localToWorldMatrix 
//			worldToLocalMatrix = getLocalToWorldMatrix().inverse(); 
//			// clear the dirty flag since the 
//			// matrix is now up to date 
//			isInverseDirty = false; 
//		} 
//		return worldToLocalMatrix; 
//	} 
//	
//	// transforms a point from local coordinates to world coordinates 
//	public Vector2 transformPoint(Vector2 point) { 
//		// matrix multiply padding the extra element with a 1 Matrix1x3 
//		transformResult = getLocalToWorldMatrix() * Matrix1x3(point.x, point.y, 1); 
//		return new Vector2(transformResult[1,1], transformResult[1,2], transformResult[1,3]); 
//	} 
//	
//	// transforms a direction from local coordinates to world coordinates 
//	public Vector2 transformDirection(Vector2 point) { 
//		// matrix multiply padding the extra element with a 0 
//		// notice that the worldToLocalMatrix is used here 
//		// and the point is multiplied as a row matrix before the 
//		// transform matrix. This is the proper way to transform 
//		// directions as described before in this article 
//		Matrix3x1 transformResult = Matrix3x1(point.x, point.y, 0) * getWorldToLocalMatrix(); 
//		return new Vector2(transformResult[1,1], transformResult[2,1], transformResult[3,1]); 
//	} 
//
//	// transforms a point from world coordinates to local coordinates 
//	public Vector2 inverseTransformPoint(Vector2 point) { 
//		// same logic as transformPoint only with the inverse matrix 
//		Matrix1x3 transformResult = getWorldToLocalMatrix() * Matrix1x3(point.x, point.y, 1); 
//		return new Vector2(transformResult[1,1], transformResult[1,2], transformResult[1,3]); 
//	} 
//	
//	// transforms a direction from world coordinates to local coordinates 
//	public Vector2 inverseTransformDirection(Vector2 point) { 
//		// same logic as transformDirection only with the inverse of the 
//		// inverse localToWorldMatrix which is just the localToWorldMatrix 
//		Matrix3x1 transformResult = Matrix3x1(point.x, point.y, 0) * getLocalToWorldMatrix(); 
//		return new Vector2(transformResult[1,1], transformResult[2,1], transformResult[3,1]); 
//	} 
//	
//	public Vector2 getLocalPosition() { 
//		return localPosition; 
//	} 
//	
//	// sets the position relative to the parent 
//	// and marks the transform as dirty 
//	public void setLocalPosition(Vector2 value) { 
//		localPosition = value; 
//		// set the dirty flag since the localToWorldMatrix needs to be updated 
//		setDirty(); 
//	} 
//	
//	/* localRoation and localScale should also have getters and setters * like the local position does. Be sure to call setDirty in the * setters for each of them */ 
//}