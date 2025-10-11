using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.GetArtists.Gateways;
using MusicDimsApi.Endpoints.Models;

namespace MusicDimsApi.Endpoints.GetRoot;

public static class GetArtistsEndpoint
{
    public static WebApplication MapGetRootEndpoint(this WebApplication app)
    {
        app.MapGet("/", () =>
        {
            var result = new NavigationResponseDto([new NavigationEntityDto("", "Artists", "/artists"), new NavigationEntityDto("", "Radio", "/radio-channels")], "/");
            return result;
        }).WithName("get-root");
 
        return app;
    } 
}