using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtistAlbums;
using MusicDimsApi.Endpoints.GetArtists;
using MusicDimsApi.Endpoints.PostPlayAlbum;
using MusicDimsApi.Endpoints.PostStopPlayBack;
using MusicDimsApi.Endpoints.PostVolumeUp;
using MusicDimsApi.Endpoints.PostVolumeDown;

var builder = WebApplication.CreateBuilder(args);

builder.Configuration.AddJsonFile("appsettings.secrets.json");
builder.AddHomeAssistantClient();
builder.AddGetArtistsEndpointServices();
builder.AddGetArtistAlbumsEndpointServices();
builder.AddPostPlayAlbumServices();
builder.AddPostStopPlayBackServices();
builder.AddPostVolumeUpServices();
builder.AddPostVolumeDownServices();

var app = builder.Build();

app.MapGetArtistsEndpoint();
app.MapGetArtistAlbumsEndpoint();
app.MapPostPlayAlbumEndpoint();
app.MapPostStopPlayBackEndpoint();
app.MapPostVolumeUpEndpoint();
app.MapPostVolumeDownEndpoint();

app.Run();