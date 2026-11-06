# 64개체 확장 전 추가 의심점·외부 군집 코드 검토 — 2026-11-06

## 1. 작업 목표
구현 전 추가 의심 지점과 다른 게임개발 환경의 실증 가능한 설계 패턴을 확인한다. 기존64실험을 보존하고새게임·새알고리즘을시작하지않는다.
## 2. 실제 수행
기존소스의근접순위·계획·정적맵·밀침delta경로와64로그를대조했다. GitHub의Recast/DetourCrowd·RVO2·MiHordeTraffic실제코드를읽고검토commit·파일SHA256·관련symbol행을external-source-review-index.json에기록했다. 저장소/전체소스복제없이메타데이터만보존했다. Godot/Unity공식문서도확인했다.
## 3. 측정 결과·추가 의심점
1. **근접순위의중첩전수순회(소스확인·실제CPU미측정):** DGBNativePursuit.FrontEligible가전체peer를순회하면서ContactUsable(peer)를호출하고,그안에서FrontRankEligible가다시전체개체를순회할수있다. 밀침의공간격자가이별도경로까지보호하지않는다. 조건성립시한계획의순위계산이제곱규모의검사를할가능성이있다. 모든호출이최악조건이라는뜻은아니다.
2. **계획예산의범위누락(소스확인):** Pursuit의0.1초최대4계획과GroundAgent의개별지면계획은별도경로다. 따라서전체경로요청이40회/초로제한된다고해석하면안된다. 실제비용표본주변약30초에GroundAgent계획진입3124회가있었다. 네이티브FindPath만3124회라는뜻은아니다.
3. **시체/정적장애물경계중복변환(소스·기록확인):** StaticMap은한갱신에서최대8개체를Apply하며각개체에대해동일Records의Bounds를다시계산한다. Bounds는충돌박스확인과8모서리의월드변환을포함한다. 기록상Records35중시체33을발견했다. 실제Bounds호출횟수/CPU는계측되지않았으며주로사망후부담후보다. 안전한벽/발지지결과캐시와다른,동일갱신내공통기하계산재사용후보다.
4. **밀침→계획갱신연결(소스확인·효과추정):** Crowd.Displace는작은접촉이동후PlanAt/NextSurfacePlan을리셋할수있다. 이미개체당0.5초간격제한이있고현재경로를즉시삭제하지않는다. 따라서매프레임전체경로삭제라는주장은틀리지만밀집한여러개체의재계획수요는늘수있다.
5. **부하와군집시간처리의상호작용(수치확인·인과미확정):** 전원생존5초후~첫피해전4214프레임중525(12.46%)가50ms초과. Crowd는dt를최대0.05초로쓴다. 같은범위의미션dt합136.72초대비min(dt,0.05)합129.522초로차이7.198초다. 이는군집밀침적분입력차이이며애니메이션이7.2초느리다거나과거애니메이션시계문제가재발했다는증거가아니다. 클램프는안전제한일수있어무작정풀면터널링·폭발적밀침위험이있다.

증가율을기하급수라고판정할근거는없다. 고정영역에서전개체쌍을본다는이론상최악조건은8명28쌍/64명2016쌍으로제곱적이다. 현재공간격자·거리·높이필터가있으므로실제쌍수는별도다. 기존8/64의부하/카메라도같지않아경험적복잡도추정에사용하지않는다.
## 4. 원인 분석
외부 근거와 적용 가능성
- **DetourCrowd:** 경로작업큐와업데이트예산,기존경로의부분재사용,이웃목록상한이실제소스에있다. 우리에게유용한것은모든계획경로의예산범위를명확히하고동일상태의순위/이웃자료를재사용하는원리다. DayZ모드에서Detour의sliced네이티브API를직접호출할수있다는증거는없다.
- **RVO2:** KdTree이웃검색과maxNeighbors로회피대상수를제한한다. 밀집군집의로컬회피비용을제어하는참고이나평면속도회피라이브러리다. 우리등반·낙하·실제월드물리를대체하지않고벽/발판검사를이웃상한때문에생략해서는안된다.
- **MiHordeTraffic:** 경로요청예산/거리별주기와오래기다린개체의기회보장,군집정체시구동·밀침완화와hold상태를실제소스에서확인했다. 계속전력으로밀고계산하는상황을줄이는참고다. 영구추적포기가아니며목표/통로변화시재시도는유지해야한다. README가한XZ열에한지면과겹친층미지원제한을명시하므로컨테이너/지면이겹치는우리씬에FlowField전체를그대로도입하는방향은부적합하다. Unity Burst/Jobs도EnforceScript에직접옮길수없다.
- **Godot공식문서:** 잦은경로갱신·너무짧은이탈임계값으로앞뒤이동/떨림이발생할수있다고설명한다. 우리밀침과재계획연결을검토할근거이지DayZ원인확정은아니다.
- **Unity공식문서:** 느린프레임의물리추격업데이트가부하를더키우는문제와프로파일링을설명한다. DayZ가동일한고정틱추격을한다는증거는없으며우리Crowd50ms클램프와구별한다. collider/broadphase·할당·sleep상태를분리하는관점도유용하지만DayZ에서같은Profiler/Job/GPUAPI를사용할수있는것은아니다.
## 5. 검증 수준
소스/기록대조및외부기술검토:진단 성공. 추가의심점의실제CPU self-time·절감률·수정후안정성은미실행/미확인. 외부프로젝트성능숫자를DayZ예상성능으로환산하지않았다.
## 6. 변경 사항
기존게임/PBO/애니메이션/실행환경불변. source index·사전검토JSON/Markdown·기존체크포인트만추가. 기각된파트축소/rootgetter후보를다시활성화하지않았다.
## 7. 원시 증거·외부 출처
ANIM-playable-sixtyfour1의server-frames.tsv·gameplay-eight.tsv·native-static-map.tsv와DGBNativePursuit/StaticMap/Crowd/GroundAgent. GitHub검토는다음고정commit을사용한다.

recastnavigation/recastnavigation
- [DetourCrowd/Source/DetourCrowd.cpp](https://github.com/recastnavigation/recastnavigation/blob/9f4ce64458dfae86e1239c525ddc219c4e9e06f1/DetourCrowd/Source/DetourCrowd.cpp)
- [DetourCrowd/Include/DetourCrowd.h](https://github.com/recastnavigation/recastnavigation/blob/9f4ce64458dfae86e1239c525ddc219c4e9e06f1/DetourCrowd/Include/DetourCrowd.h)

snape/RVO2
- [src/Agent.cc](https://github.com/snape/RVO2/blob/6b4eb00a93a9f3abf723235ff006d6aad3e2f212/src/Agent.cc)
- [src/KdTree.cc](https://github.com/snape/RVO2/blob/6b4eb00a93a9f3abf723235ff006d6aad3e2f212/src/KdTree.cc)

innocentmiau/MiHordeTraffic
- [Runtime/Pathing/HordePathScheduler.cs](https://github.com/innocentmiau/MiHordeTraffic/blob/3757971794a71a51e903a7584a5c2c9cdf2bee16/Runtime/Pathing/HordePathScheduler.cs)
- [Runtime/Movement/HordeFlowMoveJob.cs](https://github.com/innocentmiau/MiHordeTraffic/blob/3757971794a71a51e903a7584a5c2c9cdf2bee16/Runtime/Movement/HordeFlowMoveJob.cs)
- [Runtime/Separation/SeparationPushJob.cs](https://github.com/innocentmiau/MiHordeTraffic/blob/3757971794a71a51e903a7584a5c2c9cdf2bee16/Runtime/Separation/SeparationPushJob.cs)
- [README.md](https://github.com/innocentmiau/MiHordeTraffic/blob/3757971794a71a51e903a7584a5c2c9cdf2bee16/README.md)

- [Godot NavigationAgents](https://docs.godotengine.org/en/stable/tutorials/navigation/navigation_using_navigationagents.html)
- [Unity physics performance issues](https://docs.unity.com/en-us/engine/6000.0/manual/physics-section/physics-overview/physics-optimization/physics-performance-issues)

## 8. 잔여 문제와 후속 우선순위
현재64CPU최적화방향을유지한다. 먼저①근접순위중첩계산을동일입력키/한갱신범위에서중복하지않을방법②전체계획수요와현재예산의범위차이를최소후보로검토할가치가있다. 순위결과는개체별clue/위치/시야/목표revision에의존하므로무조건전역캐시하거나갱신주기만늦추면안된다. 그다음정적경계의동일갱신내재사용을검토한다. 재시도중이동유지·정상밀침·낙하·재등반·실제벽/발판검사는보존한다. 이보고서는구현승인이나설계교체가아니며새시험을시작하지않았다.
