function createStimFromImage(inFile, outFile, mode)
if nargin<2,outFile='../input/image.dat';end
if nargin<3,mode='gray';end

pic = PictureStim(inFile);
pic.resize([256 256]);

switch lower(mode)
	case 'gray'
		pic.rgb2gray();
	case 'rgb'
		pic.gray2rgb();
end

pic.save(outFile);

end