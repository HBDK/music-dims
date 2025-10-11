using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtistAlbums;
using MusicDimsApi.Endpoints.GetArtists;
using MusicDimsApi.Endpoints.PostPlayAlbum;

var builder = WebApplication.CreateBuilder(args);
builder.Configuration.AddJsonFile("appsettings.secrets.json");
builder.AddHomeAssistantClient();
builder.AddGetArtistsEndpointServices();
builder.AddGetArtistAlbumsEndpointServices();
builder.AddPostPlayAlbumServices();

var app = builder.Build();

app.MapGetArtistsEndpoint();
app.MapGetArtistAlbumsEndpoint();
app.MapPostPlayAlbumEndpoint();
app.Run();