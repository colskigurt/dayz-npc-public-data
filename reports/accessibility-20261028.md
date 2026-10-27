# DayZ NPC Observatory — 외부 AI 접근성 진단

진단일: 2026-10-28 KST. 대상은 기존 공개 Site이며 게임·PBO·실험 실행은 변경하지 않았다.
판정: **공개 읽기 인터페이스 배포·HTTP 진단 성공 / 외부 AI URL 접근 최종 수용 미완료**.

## 1. 작업 목표

사람용 대시보드와 실험 근거를 보존하면서 URL만으로 읽을 수 있는 JSON/Markdown을 제공하고, 전송 실패·렌더링 문제·외부 도구 제한을 구분한다.

## 2. 실제 수행

- 공개 정적 Site에 `/api/public/status.json`, `experiments.json`, `metrics.json`, `schema.json`, `update-status.json` 및 `/reports/latest.md` 추가.
- 루트 HTML에 JavaScript 없는 요약·링크 추가. 기존 화면에는 자료 시각·stale·버전 불일치·갱신 실패 상태 배너 추가.
- JSON Schema와 개인정보 검사, staging 검증, 로컬 교환/롤백, 실패 상태 분리 구현. 근거 원문은 기존 공개 선정 패킷에만 연결.
- 5개 자동 검사: 유효한 투영/기존 기록 보존, 잘못된 입력의 마지막 정상 자료 보존, stale 계산, 스키마/민감 정보 거부, 수치 변경의 HTML/Markdown 반영. 모두 통과.
- 기존 대시보드 scope/metrics/8섹션/선정 함수/HTTP 다운로드 검사 통과. 새 NPC 동작·성능 실험은 수행하지 않음.

## 3. 측정 결과

배포 version 3, 소스 `6052d5a09b50a013ca5285f97dd573192b56c693`에서 다음 실측을 완료했다. 후속 문서 추가는 같은 실험 스냅샷을 유지한다.

|항목|수정 전|배포 후 실측|
|---|---|---|
|A DNS|104.18.22.186 / 104.18.23.186|동일, 시스템·Google DoH 일치|
|AAAA|주소 없음(11004)|동일; 전체 DNS 실패 아님|
|TLS|검증 성공 TLS1.3|성공 TLS1.3 / TLS_AES_256_GCM_SHA384|
|루트, 브라우저형 UA|200 text/html, 수치 없음|200 text/html, 비-JS 수치·자료 시각 있음|
|API 3종|200 text/html SPA 대체 응답|200 application/json, 파싱·스키마 통과|
|Markdown|200 text/html SPA 대체 응답|200 text/markdown, 본문·시각 일치|
|robots.txt|200 text/html|200 text/plain, User-agent:* / Allow:/|
|기본 Python urllib UA|403, error code:1010|루트·status.json·latest.md에서도 동일|
|정체를 밝힌 DayZNPC-PublicReader/1.0|기존 JSON 읽기 가능|공개 API/Markdown/선정 근거 GET 모두 성공|
|외부 AI 웹 가져오기 도구|URL not accessible|루트·JSON·Markdown 모두 같은 실패; HTTP 상태 미제공|

인증 헤더·쿠키를 보내지 않았고 성공한 GET에서 리다이렉트·로그인 게이트가 없었다. 34개 지표 각각 실행 ID·빌드·조건·반복 횟수 포함. 3 JSON, Markdown, 화면 catalog의 snapshotId/generatedAt 일치. 일반 URL과 캐시 확인 쿼리 URL, 고정 스냅샷의 본문 일치. 선정 근거 6개 해시 일치.

snapshotId: `snapshot-20261027T185443179320Z-acd7bf8c`

generatedAt: `2026-10-27T18:54:43.179320Z`

sourceUpdatedAt: `2026-10-27T17:36:00.756451Z`

staleAfter: `2026-10-28T17:36:00.756451Z`

생성은 새 실험이 아니다. 현재 UTC가 staleAfter를 넘으면 오래된 자료로 판단한다. 검증 시점에는 stale=false였다. 게임의 8/64명 안정성, p50/p95/p99 또는 시각 품질을 이번 사이트 작업으로 개선했다고 주장하지 않는다.

## 4. 원인 분석

**확인된 사실**: 기존 루트는 JS 실행 후에만 실험 본문을 구성했고, 미구현 JSON/Markdown/robots 경로는 HTML fallback이었다. 이는 읽기 인터페이스 추가로 해소했다. 기본 urllib에 대한 Cloudflare1010 차단은 별개로 재현되며 남아 있다. 이름을 밝힌 사용자 에이전트의 HTTP 성공은 실제 GPTBot 성공을 뜻하지 않는다.

**추정/미확정**: Cloudflare 문서는1010을 브라우저 서명/무결성 검사와 연결한다. 그러나 이 Site의 정확한 WAF 규칙은 열람하지 못했으므로 규칙명·원인 설정을 확정할 수 없다. Sites 제공 도구에는 WAF/BIC 조정 기능이 없다. 외부 AI 웹 도구는 HTTP·DNS 정보를 주지 않아 그 실패를1010 또는 DNS 문제로 단정하지 않는다.

**반증/제외**: 사용자 네트워크의 전체 DNS 실패, TLS 인증서 오류, 공개 GET의 로그인 강제는 해당 시험에서 재현되지 않았다. 비-JS 본문 부재는 수정됐지만 외부 도구 실패는 계속돼 그것만으로 전체 원인을 설명하지 못한다.

## 5. 검증 수준

- 진단 성공: DNS/TLS/HTTP/MIME/비-JS/스키마/데이터 일치.
- 회귀 통과: 5개 새 자동 검사와 기존 정적 대시보드 검사.
- 실제 공개 HTTPS GET 성공: 사용자 컴퓨터 네트워크에서 공개 호스팅을 대상으로 수행. localhost 성공으로 대신하지 않음.
- 반복 검증: 서로 다른 UA 및 별도 진단 스크립트로 응답 분류. 장기 가용성 또는 여러 독립 네트워크의 반복 수용은 아님.
- 최종 수용: **외부 AI 접근성 미수용**. 외부 AI 웹 도구 실패가 남아 있음.
- 로컬 HTTP 검사: Windows Python 미리보기의 `.md` MIME이 text/markdown이 아니어서 실패. 실제 배포에서는 Markdown MIME 성공. 로컬 실패를 숨기거나 외부 성공의 근거로 사용하지 않음.

## 6. 변경 사항

추가: `build_public_snapshot.py`, `public_contract.py`, `test_public_snapshot.py`, `diagnose_access.py`, `verify_public_http.py`, `dist/public-reader.js`, `dist/public-reader.css`, API·Markdown·robots 정적 파일.

수정: `dist/index.html`(비-JS 요약), `dist/data/index.json` 및 `dist/downloads/catalog.json`(공개 스냅샷 메타데이터), README. 기존 `dist/app.js`, `dist/app.css`, 실험별 JSON과 선정 패킷은 보존.

`dist/_headers`의 no-store/CORS/X-Robots-Tag는 실제 Sites 정적 응답에 적용되지 않았다. 실제 Cache-Control은 `public, max-age=0, must-revalidate`; X-Robots-Tag와 CORS 허용 헤더는 없었다. 헤더 설정 성공으로 보고하지 않는다. robots Allow는 크롤링 허가 표현일 뿐 CDN 차단 해제가 아니다.

## 7. 원시 증거

로컬 비공개 진단 파일명: `diagnostics/access-before-20261028.json`, `access-after-20261028.json`, `http-public-20261028.json`, `default-client-20261028.json`, `external-ai-20261028.json`, `http-local-20261028.json`. 전체 로컬 경로·계정·프로세스·자격증명은 공개하지 않는다. 기존 checkpoints 체계에 해시 검증된 보고서와 증거 목록을 별도 보존한다.

- [대시보드](https://dayz-npc-observatory.sappyvole8.chatgpt.site/)
- [상태 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/status.json)
- [실험 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/experiments.json)
- [지표 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/metrics.json)
- [JSON 스키마](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/schema.json)
- [실험 요약 Markdown](https://dayz-npc-observatory.sappyvole8.chatgpt.site/reports/latest.md)
- [Cloudflare1010 공식 설명](https://developers.cloudflare.com/support/troubleshooting/http-status-codes/cloudflare-1xxx-errors/error-1010/)
- [정적 자산 헤더 공식 설명](https://developers.cloudflare.com/workers/static-assets/headers/)

## 8. 잔여 문제

1. 플랫폼 운영자에게1010과 외부 AI 도구 실패를 전달해 실제 보안 이벤트/봇 정책을 확인해야 한다. 현재 도구로 변경 가능한 설정은 없다. 보안 검사를 우회하거나 인증 토큰을 공개하지 않았다.
2. 별도 네트워크의 curl/AI 클라이언트 검증이 필요하다. Google DoH는 별도 DNS 해석기 증거이며 두 번째 원본 HTTP 접속 환경을 뜻하지 않는다.
3. 이 호스팅에서 외부 AI 접근을 계속 허용하지 않으면, 검증된 정적 공개 스냅샷만 별도 호스팅에 미러하는 다음 단계를 검토한다. 이번 작업에서 새 플랫폼은 생성하지 않았다.
4. 정적 사이트는 새 게시가 없는 갱신 실패를 스스로 알 수 없다. 마지막 정상 자료와 마지막 실패 상태는 다음 게시에 함께 반영해야 한다. 현재 snapshot의 updateState는 해당 스냅샷 생성 시점이며 별도 update-status.json은 가장 최근에 게시된 시도 상태다.
5. 전체 실험 결과의 공개 수용·20ms 목표·애니메이션 원인 해결은 이 접근성 진단과 별도이다.
